#pragma once
#include "RNSharedElementDrawable.h"

namespace rnoh {
class RNSharedElementView : public ArkUINode {
public:
    RNSharedElementDrawable mDrawable;
    RNSharedElementDrawable::ViewType mViewType = RNSharedElementDrawable::ViewType::NONE;

    RNSharedElementView() : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_STACK)) {
        maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_ON_CLICK, 0, this));
        maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_ON_HOVER, 0, this));

        maybeThrow(NativeNodeApi::getInstance()->addChild(m_nodeHandle, mDrawable.getArkUINodeHandle()));
    }

    ~RNSharedElementView() {
        NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_ON_CLICK);
        NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_ON_HOVER);
    }

    bool hasOverlappingRendering() const { return mViewType == RNSharedElementDrawable::ViewType::GENERIC; }

    void reset() { setAlpha(0.0f); }

    void updateViewAndDrawable(const std::shared_ptr<drawing::Rect> &layout,
                               const std::shared_ptr<drawing::Rect> &parentLayout,
                               const std::shared_ptr<drawing::Rect> &originalLayout,
                               const std::shared_ptr<drawing::Rect> &originalFrame,
                               const std::shared_ptr<RNSharedElementContent> content,
                               const std::shared_ptr<RNSharedElementStyle> style, float alpha, int resize, int align,
                               float position) {

        DLOG(INFO) << "RNSharedElementView updateViewAndDrawable";
        // Update drawable
        auto viewType = mDrawable.update(content, style, position);
        bool useGPUScaling = (resize != static_cast<int>(RNSharedElementResize::CLIP)) &&
                             (viewType == RNSharedElementDrawable::ViewType::GENERIC ||
                              viewType == RNSharedElementDrawable::ViewType::PLAIN);
        // Update layer type
        if (mViewType != viewType) {
            mViewType = viewType;
            //             setLayerType(useGPUScaling ? LAYER_TYPE_HARDWARE : LAYER_TYPE_NONE);
        }

        // Update view size/position/scale
        std::shared_ptr<drawing::Rect> layout1 = layout;
        float width = layout1->GetWidth();
        float height = layout1->GetHeight();
        //         if (useGPUScaling) {
        //             int originalWidth = originalFrame.width();
        //             int originalHeight = originalFrame.height();
        //
        //             // 更新视图
        //             layoutView(0, 0, originalWidth, originalHeight);
        //             setTranslationX(layout.left - parentLayout.left);
        //             setTranslationY(layout.top - parentLayout.top);
        //
        //             // 更新缩放
        //             float scaleX = width / originalWidth;
        //             float scaleY = height / originalHeight;
        //             if (!std::isinf(scaleX) && !std::isnan(scaleX) &&
        //                 !std::isinf(scaleY) && !std::isnan(scaleY)) {
        //
        //                 switch (resize) {
        //                     case SharedElementResize::AUTO:
        //                     case SharedElementResize::STRETCH:
        //                         break;
        //                     case SharedElementResize::CLIP:
        //                     case SharedElementResize::NONE:
        //                         scaleX = static_cast<float>(originalWidth) / originalLayout.width();
        //                         scaleY = static_cast<float>(originalHeight) / originalLayout.height();
        //                         break;
        //                 }
        //
        //                 setScaleX(scaleX);
        //                 setScaleY(scaleY);
        //             }
        //             setPivotX(0);
        //             setPivotY(0);
        //         } else {
        //             // 更新视图
        //             layoutView(0, 0, static_cast<int>(std::ceil(width)), static_cast<int>(std::ceil(height)));
        //             setTranslationX(layout.left - parentLayout.left);
        //             setTranslationY(layout.top - parentLayout.top);
        //         }

        // 更新视图的不透明度和高度
        setAlpha(alpha);
        //         if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
        //             setElevation(style.elevation);
        //         }
    }

    // 以下是伪代码，实际实现需要具体细节
    void setAlpha(float alpha) {
        // 设置透明度
    }

    void setLayerType(int layerType, void *params) {
        // 设置层类型
    }

    void layoutView(int left, int top, int right, int bottom) {
        // 布局视图
    }

    void setTranslationX(float translationX) {
        // 设置 X 方向的平移
    }

    void setTranslationY(float translationY) {
        // 设置 Y 方向的平移
    }

    void setScaleX(float scaleX) {
        // 设置 X 方向的缩放
    }

    void setScaleY(float scaleY) {
        // 设置 Y 方向的缩放
    }

    void setPivotX(float pivotX) {
        // 设置 X 轴的支点
    }

    void setPivotY(float pivotY) {
        // 设置 Y 轴的支点
    }

    // 更多方法...
};
}; // namespace rnoh