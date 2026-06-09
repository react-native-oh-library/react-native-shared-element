/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

#pragma once
#include "RNSharedElementDrawable.h"
#include "RNOH/arkui/NativeNodeApi.h"

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

    void reset() { setOpacity(0.0f); }

    void updateViewAndDrawable(const std::shared_ptr<drawing::Rect> &layout,
                               const std::shared_ptr<drawing::Rect> &parentLayout,
                               const std::shared_ptr<drawing::Rect> &originalLayout,
                               const std::shared_ptr<drawing::Rect> &originalFrame,
                               const std::shared_ptr<RNSharedElementContent> content,
                               const std::shared_ptr<RNSharedElementStyle> style, float alpha, int resize, int align,
                               float position) {

        DLOG(INFO) << "RNSharedElementView updateViewAndDrawable";
        auto viewType = mDrawable.update(content, style, position);
        bool useGPUScaling = (resize != static_cast<int>(RNSharedElementResize::CLIP)) &&
                             (viewType == RNSharedElementDrawable::ViewType::GENERIC ||
                              viewType == RNSharedElementDrawable::ViewType::PLAIN);
        if (mViewType != viewType) {
            mViewType = viewType;
        }

        std::shared_ptr<drawing::Rect> layout1 = layout;
        float width = layout1->GetWidth();
        float height = layout1->GetHeight();
        
        if (useGPUScaling) {
            float originalWidth = originalFrame->GetWidth();
            float originalHeight = originalFrame->GetHeight();

            setSize({static_cast<float>(originalWidth), static_cast<float>(originalHeight)});
            setPosition({layout1->GetLeft() - parentLayout->GetLeft(), 
                         layout1->GetTop() - parentLayout->GetTop()});

            float scaleX = width / originalWidth;
            float scaleY = height / originalHeight;
            if (!std::isinf(scaleX) && !std::isnan(scaleX) &&
                !std::isinf(scaleY) && !std::isnan(scaleY)) {
                switch (resize) {
                    case static_cast<int>(RNSharedElementResize::AUTO):
                    case static_cast<int>(RNSharedElementResize::STRETCH):
                        break;
                    case static_cast<int>(RNSharedElementResize::CLIP):
                    case static_cast<int>(RNSharedElementResize::NONE):
                        scaleX = static_cast<float>(originalWidth) / originalLayout->GetWidth();
                        scaleY = static_cast<float>(originalHeight) / originalLayout->GetHeight();
                        break;
                }
            }
        } else {
            setSize({static_cast<float>(std::ceil(width)), static_cast<float>(std::ceil(height))});
            setPosition({layout1->GetLeft() - parentLayout->GetLeft(), 
                         layout1->GetTop() - parentLayout->GetTop()});
        }

        setOpacity(alpha);
    }

    void setLayerType(int layerType, void *params) {
    }
};
}; // namespace rnoh