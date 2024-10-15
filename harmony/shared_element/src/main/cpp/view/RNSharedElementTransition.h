//
// Created on 2024/9/26.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".
#pragma once

#include "RNSharedElementNodeManager.h"
#include "RNSharedElementTransitionItem.h"
#include "RNSharedElementView.h"

#include <memory>
#include <vector>

namespace rnoh {
class RNSharedElementTransition : public ArkUINode {
public:
    enum Item { START = 0, END = 1 };
    RNSharedElementTransition();
    ~RNSharedElementTransition() override;

    void setUp(std::shared_ptr<RNSharedElementNodeManager> nodeManager, std::shared_ptr<SharedElementTurbo> turbo);

    void setItemNode(Item item, std::shared_ptr<RNSharedElementNode> node) {
        transitionItems.at(static_cast<int>(item))->setNode(node);
        requestStylesAndContent(false);
    }

    void setAnimation(int animation) {
        if (animation_ != animation) {
            animation_ = animation;
        }
    }

    void setResize(int resize) {
        if (resize_ != resize) {
            resize_ = resize;
        }
    }

    void setAlign(int align) {
        if (align_ != align) {
            align_ = align;
        }
    }

    void setNodePosition(float nodePosition) {
        if (nodePosition_ != nodePosition) {
            nodePosition_ = nodePosition;
            mInitialNodePositionSet = true;
        }
    }

    void onLayout() {
        if (!mReactLayoutSet) {
            mReactLayoutSet = true;

            // Wait for the whole layout pass to have completed before
            // requesting the layout and content
            requestStylesAndContent(true);
            mInitialLayoutPassCompleted = true;
            updateLayout();
            updateNodeVisibility();
        }
    }

    std::shared_ptr<RNSharedElementNodeManager> getNodeManager() { return nodeMgr; }

    using Weak = std::weak_ptr<ComponentInstance>;
    ComponentInstance::Weak parent;

private:
    int animation_;
    int resize_;
    int align_;
    float nodePosition_;
    bool mInitialNodePositionSet = false;
    bool mInitialLayoutPassCompleted = false;
    std::vector<std::shared_ptr<RNSharedElementTransitionItem>> transitionItems;
    std::shared_ptr<RNSharedElementNodeManager> nodeMgr;
    std::vector<int> mParentOffset;
    int mInitialVisibleAncestorIndex = -1;
    bool mRequiresClipping = false;
    bool mReactLayoutSet = false;
    RNSharedElementView mStartView;
    RNSharedElementView mEndView;

    static std::shared_ptr<drawing::Rect> getClipInsets(std::shared_ptr<drawing::Rect> layout, std::shared_ptr<drawing::Rect> clippedLayout) {
        if (layout == nullptr) {
            layout = std::make_shared<drawing::Rect>();
        }
        if (clippedLayout == nullptr) {
            clippedLayout = std::make_shared<drawing::Rect>();
        }
        std::shared_ptr<drawing::Rect> rect = std::make_shared<drawing::Rect>(clippedLayout->GetLeft() - layout->GetLeft(), clippedLayout->GetTop() -layout->GetTop(),
                          layout->GetRight() - clippedLayout->GetRight(),
                          layout->GetBottom() - clippedLayout->GetBottom());
        return rect;
    }

    static std::shared_ptr<drawing::Rect> getInterpolatedClipInsets(std::shared_ptr<drawing::Rect> interpolatedLayout, std::shared_ptr<drawing::Rect> startClipInsets,
                                                   std::shared_ptr<drawing::Rect> startClippedLayout, std::shared_ptr<drawing::Rect> endClipInsets,
                                                   std::shared_ptr<drawing::Rect> endClippedLayout, float position) {
        if (interpolatedLayout == nullptr) {
            interpolatedLayout = std::make_shared<drawing::Rect>();
        }
        if (startClipInsets == nullptr) {
            startClipInsets = std::make_shared<drawing::Rect>();
        }
        if (startClippedLayout == nullptr) {
            startClippedLayout = std::make_shared<drawing::Rect>();
        }
        if (endClipInsets == nullptr) {
            endClipInsets = std::make_shared<drawing::Rect>();
        }
        if (endClippedLayout == nullptr) {
            endClippedLayout = std::make_shared<drawing::Rect>();
        }
        std::shared_ptr<drawing::Rect> clipInsets = std::make_shared<drawing::Rect>();

        // Top
        if ((endClipInsets->GetTop() == 0) && (startClipInsets->GetTop() != 0) &&
            (startClippedLayout->GetTop() <= endClippedLayout->GetTop())) {
            clipInsets->SetTop(fmax(0, startClippedLayout->GetTop() - interpolatedLayout->GetTop()));
        } else if ((startClipInsets->GetTop() == 0) && (endClipInsets->GetTop() != 0) &&
                   (endClippedLayout->GetTop() <= startClippedLayout->GetTop())) {
            clipInsets->SetTop(fmax(0, endClippedLayout->GetTop() - interpolatedLayout->GetTop()));
        } else {
            clipInsets->SetTop(
                (startClipInsets->GetTop() + ((endClipInsets->GetTop() - startClipInsets->GetTop()) * position)));
        }

        // Bottom
        if ((endClipInsets->GetBottom() == 0) && (startClipInsets->GetBottom() != 0) &&
            (startClippedLayout->GetBottom() >= endClippedLayout->GetBottom())) {
            clipInsets->SetBottom(fmax(0, interpolatedLayout->GetBottom() - startClippedLayout->GetBottom()));
        } else if ((startClipInsets->GetBottom() == 0) && (endClipInsets->GetBottom() != 0) &&
                   (endClippedLayout->GetBottom() >= startClippedLayout->GetBottom())) {
            clipInsets->SetBottom(fmax(0, interpolatedLayout->GetBottom() - endClippedLayout->GetBottom()));
        } else {
            clipInsets->SetBottom(startClipInsets->GetBottom() +
                                 ((endClipInsets->GetBottom() - startClipInsets->GetBottom()) * position));
        }

        // Left
        if ((endClipInsets->GetLeft() == 0) && (startClipInsets->GetLeft() != 0) &&
            (startClippedLayout->GetLeft() <= endClippedLayout->GetLeft())) {
            clipInsets->SetLeft(fmax(0, startClippedLayout->GetLeft() - interpolatedLayout->GetLeft()));
        } else if ((startClipInsets->GetLeft() == 0) && (endClipInsets->GetLeft() != 0) &&
                   (endClippedLayout->GetLeft() <= startClippedLayout->GetLeft())) {
            clipInsets->SetLeft(fmax(0, endClippedLayout->GetLeft() - interpolatedLayout->GetLeft()));
        } else {
            clipInsets->SetLeft(startClipInsets->GetLeft() +
                               ((endClipInsets->GetLeft() - startClipInsets->GetLeft()) * position));
        }

        // Right
        if ((endClipInsets->GetRight() == 0) && (startClipInsets->GetRight() != 0) &&
            (startClippedLayout->GetRight() >= endClippedLayout->GetRight())) {
            clipInsets->SetRight(fmax(0, interpolatedLayout->GetRight() - startClippedLayout->GetRight()));
        } else if ((startClipInsets->GetRight() == 0) && (endClipInsets->GetRight() != 0) &&
                   (endClippedLayout->GetRight() >= startClippedLayout->GetRight())) {
            clipInsets->SetRight(fmax(0, interpolatedLayout->GetRight() - endClippedLayout->GetRight()));
        } else {
            clipInsets->SetRight(startClipInsets->GetRight() +
                                ((endClipInsets->GetRight() - startClipInsets->GetRight()) * position));
        }

        return clipInsets;
    }

    void requestStylesAndContent(bool force) {
        if (!mInitialLayoutPassCompleted && !force)
            return;
        
        for (std::shared_ptr<RNSharedElementTransitionItem> item : transitionItems) {
            auto node = item->getNode();
             DLOG(WARNING) << "requestStylesAndContent item node: " << node;
            if (item->getNeedsStyle()) {
                item->setNeedsStyle(false);
                if (node) {
                    node->requestStyle([&item, this](std::shared_ptr<RNSharedElementStyle> style) {
                        if (style) {
                            item->setStyle(style);
                            updateLayout();
                            updateNodeVisibility();
                        }
                    });
                }
            }
            if (item->getNeedsContent()) {
                item->setNeedsContent(false);
                if (node) {
                    node->requestContent([&item, this](std::shared_ptr<RNSharedElementContent> content) {
                        if (content) {
                            item->setContent(content);
                            updateLayout();
                            updateNodeVisibility();
                        }
                    });
                }
            }
        }
    };

    void updateLayout() {
        if (!mInitialLayoutPassCompleted)
            return;

        // Local data
        std::shared_ptr<RNSharedElementTransitionItem> startItem = transitionItems.at(static_cast<int>(START));
        std::shared_ptr<RNSharedElementTransitionItem> endItem = transitionItems.at(static_cast<int>(END));

        // Get parent offset
        // todo
        // parent.getLocationInWindow(mParentOffset);
        mParentOffset = {0, 0};


        // Get styles
        std::shared_ptr<RNSharedElementStyle> startStyle = startItem->getStyle();
        auto endStyle = endItem->getStyle();
        if ((startStyle == nullptr) && (endStyle == nullptr)) {
            DLOG(WARNING) << "updateLayout is return, startStyle: " << startStyle << " endStyle: " << endStyle;
            return;
        }


        // Get content
        auto startContent = startItem->getContent();
        auto endContent = endItem->getContent();
        if ((animation_ == static_cast<int>(RNSharedElementAnimation::MOVE)) && (startContent == nullptr) &&
            (endContent != nullptr)) {
            startContent = endContent;
        }
        DLOG(INFO) << "updateLayout startContent: " << startContent << " endContent: " << endContent;


        // Determine starting scene that is currently visible to the user
        if (mInitialVisibleAncestorIndex < 0) {
            if ((startStyle != nullptr) && (endStyle == nullptr)) {
                mInitialVisibleAncestorIndex = (endItem->getNode() == nullptr) ? 1 : 0;
            } else if ((endStyle != nullptr) && (startStyle == nullptr)) {
                mInitialVisibleAncestorIndex = (startItem->getNode() == nullptr) ? 0 : 1;
            } else if ((startStyle != nullptr) && (endStyle != nullptr)) {
                float startAncestorVisibility = RNSharedElementStyle::getAncestorVisibility(parent, startStyle);
                float endAncestorVisibility = RNSharedElementStyle::getAncestorVisibility(parent, endStyle);
                mInitialVisibleAncestorIndex = endAncestorVisibility > startAncestorVisibility ? 1 : 0;
            } else {
                // Wait for both styles before deciding which ancestor is currently visible to the user
            }
        }

        //std::shared_ptr<drawing::Rect> emptyRect = std::make_shared<drawing::Rect>();
        std::shared_ptr<drawing::Rect> startEmptyRect = std::make_shared<drawing::Rect>();
        std::shared_ptr<drawing::Rect> endEmptyRect = std::make_shared<drawing::Rect>();
        std::shared_ptr<drawing::Rect> startClipEmptyRect = std::make_shared<drawing::Rect>();
        std::shared_ptr<drawing::Rect> endClipEmptyRect = std::make_shared<drawing::Rect>();

        // Get layout
        bool startCompensate = mInitialVisibleAncestorIndex == 1;
        std::shared_ptr<drawing::Rect> startLayout = RNSharedElementStyle::normalizeLayout(startCompensate, startStyle, mParentOffset);
        std::shared_ptr<drawing::Rect> startFrame = (startStyle != nullptr) ? startStyle->frame : startEmptyRect;
        bool endCompensate = mInitialVisibleAncestorIndex == 0;
        std::shared_ptr<drawing::Rect> endLayout = RNSharedElementStyle::normalizeLayout(endCompensate, endStyle, mParentOffset);
        std::shared_ptr<drawing::Rect> endFrame = (endStyle != nullptr) ? endStyle->frame : endEmptyRect;

        // Get clipped areas
        std::shared_ptr<drawing::Rect> startClippedLayout = RNSharedElementStyle::normalizeLayout(
            startCompensate, (startStyle != nullptr) ? startItem->getClippedLayout() : startClipEmptyRect, startStyle,
            mParentOffset);
        std::shared_ptr<drawing::Rect> startClipInsets = getClipInsets(startLayout, startClippedLayout);
        std::shared_ptr<drawing::Rect> endClippedLayout = RNSharedElementStyle::normalizeLayout(
            endCompensate, (endStyle != nullptr) ? endItem->getClippedLayout() : endClipEmptyRect, endStyle, mParentOffset);
        std::shared_ptr<drawing::Rect> endClipInsets = getClipInsets(endLayout, endClippedLayout);

        // Get interpolated layout
        std::shared_ptr<drawing::Rect> interpolatedLayout = nullptr;
        std::shared_ptr<drawing::Rect> interpolatedClipInsets = nullptr;
        std::shared_ptr<RNSharedElementStyle> interpolatedStyle = nullptr;
        if ((startStyle != nullptr) && (endStyle != nullptr)) {
            interpolatedLayout = RNSharedElementStyle::getInterpolatedLayout(startLayout, endLayout, nodePosition_);
            interpolatedClipInsets = getInterpolatedClipInsets(interpolatedLayout, startClipInsets, startClippedLayout,
                                                               endClipInsets, endClippedLayout, nodePosition_);
            interpolatedStyle = RNSharedElementStyle::getInterpolatedStyle(startStyle, startLayout, endStyle, endLayout, nodePosition_);
        } else if (startStyle != nullptr) {
            interpolatedLayout = startLayout;
            interpolatedStyle = startStyle;
            interpolatedClipInsets = startClipInsets;
        } else {
            if (!mInitialNodePositionSet) {
                nodePosition_ = 1.0f;
                mInitialNodePositionSet = true;
            }
            interpolatedLayout = endLayout;
            interpolatedStyle = endStyle;
            interpolatedClipInsets = endClipInsets;
        }

        // Calculate outer frame rect. Apply clipping insets if needed
        std::shared_ptr<drawing::Rect> parentLayout;
        if (interpolatedClipInsets->GetLeft() > 0.0f || interpolatedClipInsets->GetTop() > 0.0f ||
            interpolatedClipInsets->GetRight() > 0.0f || interpolatedClipInsets->GetBottom() > 0.0f) {
            parentLayout = std::make_shared<drawing::Rect>(interpolatedLayout->GetLeft(), interpolatedLayout->GetTop(), interpolatedLayout->GetRight(), interpolatedLayout->GetBottom());
            
            parentLayout->SetLeft(parentLayout->GetLeft() + interpolatedClipInsets->GetLeft());
            parentLayout->SetTop(parentLayout->GetTop() + interpolatedClipInsets->GetTop());
            parentLayout->SetRight(parentLayout->GetRight() + interpolatedClipInsets->GetRight());
            parentLayout->SetBottom(parentLayout->GetBottom() + interpolatedClipInsets->GetBottom());
            mRequiresClipping = true;
        } else {
            parentLayout = std::make_shared<drawing::Rect>(startLayout->GetLeft(), startLayout->GetTop(), startLayout->GetRight(), startLayout->GetBottom());
            
            parentLayout->Join(*endLayout);
            mRequiresClipping = false;
        }

        // Update outer viewgroup layout. The outer viewgroup hosts 2 inner views
        // which draw the content & elevation. The outer viewgroup performs additional
        // clipping on these views.
        // todo 调用view接口
        /*super.layout(-mParentOffset[0], -mParentOffset[1], (int) Math.ceil(parentLayout->width() - mParentOffset[0]),
                     (int)Math.ceil(parentLayout->height() - mParentOffset[1]));
        setTranslationX(parentLayout->left);
        setTranslationY(parentLayout->top);*/

        // Determine opacity
        float startAlpha = 1.0f;
        float endAlpha = 1.0f;

        switch (animation_) {
        case 0:
            startAlpha = interpolatedStyle->opacity;
            endAlpha = (startStyle == nullptr) ? interpolatedStyle->opacity : 0.0f;
            break;
        case 1:
            startAlpha = ((startStyle != nullptr) ? startStyle->opacity : 1) * (1 - nodePosition_);
            endAlpha = ((endStyle != nullptr) ? endStyle->opacity : 1) * nodePosition_;
            break;
        case 2:
            startAlpha = 0.0f;
            endAlpha = ((endStyle != nullptr) ? endStyle->opacity : 1) * nodePosition_;
            break;
        case 3:
            startAlpha = ((startStyle != nullptr) ? startStyle->opacity : 1) * (1 - nodePosition_);
            endAlpha = 0.0f;
            break;
        }

        DLOG(INFO) << "SharedElementTransition updateLayout Render the start and end view";
        // Render the start view
        if (animation_ != static_cast<int>(RNSharedElementAnimation::FADE_IN)) {
            mStartView.updateViewAndDrawable(interpolatedLayout, parentLayout, startLayout, startFrame, startContent,
                                             interpolatedStyle, startAlpha, resize_, align_, nodePosition_);
        }

        // Render the end view as well for the "cross-fade" animations
        if ((animation_ == static_cast<int>(RNSharedElementAnimation::FADE)) ||
            (animation_ == static_cast<int>(RNSharedElementAnimation::FADE_IN)) ||
            ((animation_ == static_cast<int>(RNSharedElementAnimation::MOVE)) && (startStyle == nullptr))) {
            mEndView.updateViewAndDrawable(interpolatedLayout, parentLayout, endLayout, endFrame, endContent,
                                           interpolatedStyle, endAlpha, resize_, align_, nodePosition_);

            // Also apply a fade effect on the elevation. This reduces the shadow visibility
            // underneath the view which becomes visible when the transparency of the view
            // is set. This in turn makes the shadow very visible and gives the whole view
            // a "grayish" appearance. The following code tries to reduce that visual artefact.
            // todo 确认是否有 elevation 属性
            /*if (interpolatedStyle->elevation > 0) {
                if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
                    mStartView.setOutlineAmbientShadowColor(Color.argb(startAlpha, 0, 0, 0));
                    mStartView.setOutlineSpotShadowColor(Color.argb(startAlpha, 0, 0, 0));
                    mEndView.setOutlineAmbientShadowColor(Color.argb(endAlpha, 0, 0, 0));
                    mEndView.setOutlineSpotShadowColor(Color.argb(endAlpha, 0, 0, 0));
                }
            }*/
        } else {
            mEndView.reset();
        }

        // Fire events
        if ((startStyle != nullptr) && !startItem->getHasCalledOnMeasure()) {
            startItem->setHasCalledOnMeasure(true);
            fireMeasureEvent("startNode", startItem, startLayout, startClippedLayout);
        }
        if ((endStyle != nullptr) && !endItem->getHasCalledOnMeasure()) {
            endItem->setHasCalledOnMeasure(true);
            fireMeasureEvent("endNode", endItem, endLayout, endClippedLayout);
        }
    }

    void fireMeasureEvent(std::string name, std::shared_ptr<RNSharedElementTransitionItem> item, std::shared_ptr<drawing::Rect> layout,
                          std::shared_ptr<drawing::Rect> clippedLayout) {
        // todo 收集数据，回调到js侧的 onMeasureNode
        std::shared_ptr<RNSharedElementStyle> style = item->getStyle();
        std::shared_ptr<RNSharedElementContent> content = item->getContent();
        /* WritableMap layoutData = Arguments.createMap();
         layoutData.putDouble("x", layout.left - mParentOffset[0]);
         layoutData.putDouble("y", PixelUtil.toDIPFromPixel(layout.top - mParentOffset[1]));
         layoutData.putDouble("width", PixelUtil.toDIPFromPixel(layout.width()));
         layoutData.putDouble("height", PixelUtil.toDIPFromPixel(layout.height()));
         layoutData.putDouble("visibleX", PixelUtil.toDIPFromPixel(clippedLayout.left - mParentOffset[0]));
         layoutData.putDouble("visibleY", PixelUtil.toDIPFromPixel(clippedLayout.top - mParentOffset[1]));
         layoutData.putDouble("visibleWidth", PixelUtil.toDIPFromPixel(clippedLayout.width()));
         layoutData.putDouble("visibleHeight", PixelUtil.toDIPFromPixel(clippedLayout.height()));
         // TODO: intrinsic content (unclipped size & position of image)
         layoutData.putDouble("contentX", PixelUtil.toDIPFromPixel(layout.left - mParentOffset[0])); // TODO
         layoutData.putDouble("contentY", PixelUtil.toDIPFromPixel(layout.top - mParentOffset[1]));  // TODO
         layoutData.putDouble("contentWidth", PixelUtil.toDIPFromPixel(layout.width()));             // TODO
         layoutData.putDouble("contentHeight", PixelUtil.toDIPFromPixel(layout.height()));           // TODO

         WritableMap styleData = Arguments.createMap();
         styleData.putDouble("borderTopLeftRadius", PixelUtil.toDIPFromPixel(style.borderTopLeftRadius));
         styleData.putDouble("borderTopRightRadius", PixelUtil.toDIPFromPixel(style.borderTopRightRadius));
         styleData.putDouble("borderBottomLeftRadius", PixelUtil.toDIPFromPixel(style.borderBottomLeftRadius));
         styleData.putDouble("borderBottomRightRadius", PixelUtil.toDIPFromPixel(style.borderBottomRightRadius));

         WritableMap eventData = Arguments.createMap();
         eventData.putString("node", name);
         eventData.putMap("layout", layoutData);
         RNSharedElementDrawable.ViewType viewType = (content != null)
                                                         ? RNSharedElementDrawable.getViewType(content.view, style)
                                                         : RNSharedElementDrawable.ViewType.NONE;
         eventData.putString("contentType", viewType.getValue());
         eventData.putMap("style", styleData);

         reactContext.getJSModule(RCTEventEmitter.class).receiveEvent(getId(), "onMeasureNode", eventData);*/
    }

    void updateNodeVisibility() {
        for (std::shared_ptr<RNSharedElementTransitionItem> item : transitionItems) {
            bool hidden =
                mInitialLayoutPassCompleted && (item->getStyle() != nullptr) && (item->getContent() != nullptr);
            if (hidden && (animation_ == static_cast<int>(RNSharedElementAnimation::FADE_IN)) &&
                item->getName().compare("start") == 0) {
                hidden = false;
            }
            if (hidden && (animation_ == static_cast<int>(RNSharedElementAnimation::FADE_OUT)) &&
                item->getName().compare("end") == 0) {
                hidden = false;
            }
            item->setHidden(hidden);
        }
    }
};
}; // namespace rnoh
