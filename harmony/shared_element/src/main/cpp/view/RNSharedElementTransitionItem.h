//
// Created on 2024/9/26.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".
#pragma once

#include "RNOH/CppComponentInstance.h"
#include "RNSharedElementContent.h"
#include "RNSharedElementNode.h"
#include "RNSharedElementNodeManager.h"
#include "RNSharedElementStyle.h"
#include "SharedElementTransitionComponentDescriptor.h"

#include <memory>

namespace rnoh {
class RNSharedElementTransitionItem {
public:
    RNSharedElementTransitionItem(std::shared_ptr<RNSharedElementNodeManager> nodeManager, std::string name)
        : mNodeManager(nodeManager), mName(name) {}
    ~RNSharedElementTransitionItem() = default;

    void setHidden(bool hidden) {
        if (mHidden == hidden)
            return;
        mHidden = hidden;
        if (mNode == nullptr)
            return;
        if (hidden) {
            mNode->addHideRef();
        } else {
            mNode->releaseHideRef();
        }
    }

    bool getHidden() { return mHidden; }

    std::shared_ptr<RNSharedElementNode> getNode() { return mNode; }

    void setNode(std::shared_ptr<RNSharedElementNode> node) {
        if (mNode == node) {
            if (node != nullptr)
                mNodeManager->release(node);
            return;
        }
        if (mNode != nullptr) {
            if (mHidden)
                mNode->releaseHideRef();
            mNodeManager->release(mNode);
        }
        mNode = node;
        mNeedsStyle = node != nullptr;
        mStyle = nullptr;
        mNeedsContent = (node != nullptr);
        mContent = nullptr;
        if (mNode != nullptr) {
            if (mHidden)
                mNode->addHideRef();
        }
    }

    bool getNeedsStyle() { return mNeedsStyle; }

    void setNeedsStyle(bool needsStyle) { mNeedsStyle = needsStyle; }

    void setStyle(std::shared_ptr<RNSharedElementStyle> style) { mStyle = style; }

    std::shared_ptr<RNSharedElementStyle> getStyle() { return mStyle; }

    bool getNeedsContent() { return mNeedsContent; }

    void setNeedsContent(bool needsContent) { mNeedsContent = needsContent; }

    void setContent(std::shared_ptr<RNSharedElementContent> content) { mContent = content; }

    std::shared_ptr<RNSharedElementContent> getContent() { return mContent; }

    void setHasCalledOnMeasure(bool hasCalledOnMeasure) { mHasCalledOnMeasure = hasCalledOnMeasure; }

    bool getHasCalledOnMeasure() { return mHasCalledOnMeasure; }

    std::shared_ptr<ComponentInstance> getView() { return (mNode != nullptr) ? mNode->getResolvedView() : nullptr; }

    std::shared_ptr<drawing::Rect> getClippedLayout() {
        if (mStyle == nullptr)
            return mClippedLayoutCache;

        std::shared_ptr<ComponentInstance> ancestorView = mNode->getAncestorView();

        // Get visible area (some parts may be clipped in a scrollview or something)
        std::shared_ptr<drawing::Rect> styleLayout = mStyle->layout;
        std::shared_ptr<drawing::Rect> clippedLayout = nullptr;
        if (styleLayout) {
            clippedLayout = std::make_shared<drawing::Rect>(styleLayout->GetLeft(), styleLayout->GetTop(), styleLayout->GetRight(), styleLayout->GetBottom());
        } else {
            clippedLayout = std::make_shared<drawing::Rect>();
        }
        std::shared_ptr<ComponentInstance> parentView = getView()->getParent().lock();
        std::shared_ptr<drawing::Rect> bounds = std::make_shared<drawing::Rect>();
        while (parentView != nullptr) {
            if (parentView->getChildren().size() == 0)
                break;
            if (parentView->isClippingSubviews()) {
                RNSharedElementStyle::getLayoutOnScreen(parentView, bounds);

                if (!clippedLayout->Intersect(*bounds)) {
                    if (clippedLayout->GetBottom() <bounds->GetTop()) {
                        clippedLayout->SetTop(bounds->GetTop());
                        clippedLayout->SetBottom(bounds->GetTop());
                    }
                    if (clippedLayout->GetTop() >bounds->GetBottom()) {
                        clippedLayout->SetTop(bounds->GetBottom());
                        clippedLayout->SetBottom(bounds->GetBottom());
                    }
                    if (clippedLayout->GetRight() <bounds->GetLeft()) {
                        clippedLayout->SetLeft(bounds->GetLeft());
                        clippedLayout->SetRight(bounds->GetLeft());
                    }
                    if (clippedLayout->GetLeft() >bounds->GetRight()) {
                        clippedLayout->SetLeft(bounds->GetRight());
                        clippedLayout->SetRight(bounds->GetRight());
                    }
                    break;
                }
            }
            if (parentView == ancestorView) {
                break;
            }
            parentView = parentView->getParent().lock();
        }
        mClippedLayoutCache = clippedLayout;
        return mClippedLayoutCache;
    }

    std::string getName() { return mName; }

private:
    std::shared_ptr<RNSharedElementNodeManager> mNodeManager;
    std::shared_ptr<RNSharedElementNode> mNode;
    bool mHidden;
    bool mNeedsStyle;
    std::shared_ptr<RNSharedElementStyle> mStyle;
    bool mNeedsContent;
    std::shared_ptr<RNSharedElementContent> mContent;
    std::shared_ptr<drawing::Rect> mClippedLayoutCache;
    bool mHasCalledOnMeasure;
    std::string mName;
};
}; // namespace rnoh
