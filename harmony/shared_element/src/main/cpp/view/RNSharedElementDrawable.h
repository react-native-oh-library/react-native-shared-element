//
// Created on 2024/9/25.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".
#pragma once

#include "RNSharedElementContent.h"
#include "RNSharedElementStyle.h"
#include "SharedElementTurbo.h"

namespace rnoh {

struct SharedEUserCallback {
    std::function<void(ArkUI_NodeCustomEvent *event)> callback;
};

class RNSharedElementDrawable : public ArkUINode {
public:
    std::shared_ptr<SharedElementTurbo> mTurboModule;
    enum class ViewType { NONE = 0, REACTIMAGEVIEW = 1, IMAGEVIEW = 2, GENERIC = 3, PLAIN = 4 };

    RNSharedElementDrawable();
    ~RNSharedElementDrawable() override;

   std::shared_ptr< RNSharedElementStyle> getStyle() { return mStyle; }

    std::shared_ptr<RNSharedElementContent> getContent() { return mContent; }

    float getPosition() { return mPosition; }

    ViewType update(std::shared_ptr<RNSharedElementContent> content, std::shared_ptr<RNSharedElementStyle> style,
                    float position);

    static ViewType getViewType(std::shared_ptr<ComponentInstance> view, std::shared_ptr<RNSharedElementStyle> style) {
        if (view == nullptr)
            return RNSharedElementDrawable::ViewType::NONE;
        auto viewName = view->getComponentName();
        if (viewName == "ReactImageView") {
            return RNSharedElementDrawable::ViewType::REACTIMAGEVIEW;
        } else if (viewName == "ImageView") {
            return RNSharedElementDrawable::ViewType::IMAGEVIEW;
        } else if (viewName == "ReactViewGroup") {
            if (view->getChildren().size() == 0) {
                if (style->isVisible()) {
                    return RNSharedElementDrawable::ViewType::PLAIN;
                } else {
                    return RNSharedElementDrawable::ViewType::NONE;
                }
            }
        }
        return RNSharedElementDrawable::ViewType::GENERIC;
    }

private:
    std::shared_ptr<RNSharedElementContent> mContent = nullptr;
    std::shared_ptr<RNSharedElementStyle> mStyle = nullptr;
    ViewType mViewType = ViewType::NONE;
    float mPosition = 0;
    

    void draw(ArkUI_NodeCustomEvent *event);
    ArkUI_NativeNodeAPI_1 *nativeModule_ = nullptr;
    SharedEUserCallback *userCallback_ = nullptr;
    void (*eventReceiver)(ArkUI_NodeCustomEvent *event);

    void drawReactImageView(OH_Drawing_Canvas *canvas);
    void drawImageView(OH_Drawing_Canvas *canvas);
    void drawPlainView(OH_Drawing_Canvas *canvas);
    void drawGenericView(OH_Drawing_Canvas *canvas);
};
} // namespace rnoh
