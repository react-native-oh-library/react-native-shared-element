//
// Created on 2024/9/25.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include "RNSharedElementDrawable.h"
#include <native_drawing/drawing_brush.h>
#include <native_drawing/drawing_canvas.h>
#include <native_drawing/drawing_point.h>
#include <native_drawing/drawing_path.h>
#include <native_drawing/drawing_round_rect.h>
#include "SharedElementManager.h"

namespace rnoh {

RNSharedElementDrawable::RNSharedElementDrawable()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_CUSTOM)) {
    userCallback_ = new SharedEUserCallback();
    userCallback_->callback = [this](ArkUI_NodeCustomEvent *event) {
        auto type = OH_ArkUI_NodeCustomEvent_GetEventType(event);
        switch (type) {
        case ARKUI_NODE_CUSTOM_EVENT_ON_DRAW:
            draw(event);
            break;
        case ARKUI_NODE_CUSTOM_EVENT_ON_MEASURE:
            DLOG(INFO) << "custom event measure w/h: " << getSavedWidth() << "/" << getSavedHeight();

            maybeThrow(NativeNodeApi::getInstance()->setMeasuredSize(m_nodeHandle, 200, 200));
            break;
        case ARKUI_NODE_CUSTOM_EVENT_ON_LAYOUT:
            DLOG(INFO) << "custom event layout x/y: " << getLayoutPosition().x << "/" << getLayoutPosition().y;
            break;
        default:
            break;
        }
    };
    eventReceiver = [](ArkUI_NodeCustomEvent *event) {
        int32_t tagId = OH_ArkUI_NodeCustomEvent_GetEventTargetId(event);
        if (tagId == 77 || tagId == 89 || tagId == 90) {
            auto *userData = reinterpret_cast<UserCallback *>(OH_ArkUI_NodeCustomEvent_GetUserData(event));
            if (userData != nullptr && userData->callback != nullptr) {
                userData->callback(event);
            }
        }
    };
    maybeThrow(NativeNodeApi::getInstance()->addNodeCustomEventReceiver(m_nodeHandle, eventReceiver));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeCustomEvent(m_nodeHandle, ARKUI_NODE_CUSTOM_EVENT_ON_DRAW, 77,
                                                                     userCallback_));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeCustomEvent(m_nodeHandle, ARKUI_NODE_CUSTOM_EVENT_ON_MEASURE,
                                                                     89, userCallback_));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeCustomEvent(m_nodeHandle, ARKUI_NODE_CUSTOM_EVENT_ON_LAYOUT,
                                                                     90, userCallback_));
}

RNSharedElementDrawable::~RNSharedElementDrawable() {
    NativeNodeApi::getInstance()->unregisterNodeCustomEvent(m_nodeHandle, ARKUI_NODE_CUSTOM_EVENT_ON_DRAW);
    NativeNodeApi::getInstance()->unregisterNodeCustomEvent(m_nodeHandle, ARKUI_NODE_CUSTOM_EVENT_ON_MEASURE);
    NativeNodeApi::getInstance()->unregisterNodeCustomEvent(m_nodeHandle, ARKUI_NODE_CUSTOM_EVENT_ON_LAYOUT);
    NativeNodeApi::getInstance()->removeNodeCustomEventReceiver(m_nodeHandle, eventReceiver);
    delete userCallback_;
    userCallback_ = nullptr;
}

RNSharedElementDrawable::ViewType RNSharedElementDrawable::update(std::shared_ptr<RNSharedElementContent> content,
                                                                  std::shared_ptr<RNSharedElementStyle> style,
                                                                  float position) {
    bool invalidated = false;
    // Update content
    if (mContent != content) {
        mContent = content;
        invalidated = true;
    }

    // Update view-type
    ViewType viewType = (mContent != nullptr) ? RNSharedElementDrawable::getViewType(mContent->view, style)
                                              : RNSharedElementDrawable::ViewType::NONE;
    if (mViewType != viewType) {
        mViewType = viewType;
        invalidated = true;
    }

    // Update & check style changes
    if ((mStyle != nullptr) && (style != nullptr) && !invalidated) {
        switch (viewType) {
        case RNSharedElementDrawable::ViewType::REACTIMAGEVIEW:
        case RNSharedElementDrawable::ViewType::IMAGEVIEW:
            // Log.d(LOG_TAG, "drawableChanged, viewType: " + viewType + ", changes: " + mStyle.compare(style));
            invalidated = (mStyle->compare(style) &
                           (RNSharedElementStyle::PROP_BORDER | RNSharedElementStyle::PROP_BACKGROUND_COLOR |
                            RNSharedElementStyle::PROP_SCALETYPE)) != 0;
            break;
        case RNSharedElementDrawable::ViewType::PLAIN:
            // Log.d(LOG_TAG, "drawableChanged, viewType: " + viewType + ", changes: " + mStyle.compare(style));
            invalidated = (mStyle->compare(style) &
                           (RNSharedElementStyle::PROP_BORDER | RNSharedElementStyle::PROP_BACKGROUND_COLOR)) != 0;
            break;
        case RNSharedElementDrawable::ViewType::GENERIC:
            // nop
            break;
        }
    }
    mStyle = style;

    // Update position
    mPosition = position;

    // Invalidate if necessary
    if (invalidated) {
        // todo invalidateSelf();
    }
    return viewType;
}

void RNSharedElementDrawable::draw(ArkUI_NodeCustomEvent *event) {
    auto *drawContext = OH_ArkUI_NodeCustomEvent_GetDrawContextInDraw(event);
    OH_Drawing_Canvas *canvas = reinterpret_cast<OH_Drawing_Canvas *>(OH_ArkUI_DrawContext_GetCanvas(drawContext));
    DLOG(INFO) << "RNSharedElementDrawable draw CanvasGetWidth/Height: " << OH_Drawing_CanvasGetWidth(canvas) << "/"
               << OH_Drawing_CanvasGetHeight(canvas) << " viewType: " << static_cast<int>(mViewType);
    const auto count = OH_Drawing_CanvasGetSaveCount(canvas);
    OH_Drawing_CanvasSave(canvas);


    auto brush = OH_Drawing_BrushCreate();
    OH_Drawing_BrushSetColor(brush, 0xffff0000);
    constexpr uint8_t alpha = 128;
    OH_Drawing_BrushSetAlpha(brush, alpha);
    OH_Drawing_CanvasAttachBrush(canvas, brush);
    mViewType = ViewType::REACTIMAGEVIEW;

    switch (mViewType) {
    case ViewType::REACTIMAGEVIEW:
        drawReactImageView(canvas);
        break;
    case ViewType::IMAGEVIEW:
        drawImageView(canvas);
        break;
    case ViewType::PLAIN:
        drawPlainView(canvas);
        break;
    case ViewType::GENERIC:
        drawGenericView(canvas);
        break;
    }


    OH_Drawing_CanvasRestoreToCount(canvas, count);
}

void RNSharedElementDrawable::drawReactImageView(OH_Drawing_Canvas *canvas) {
    // 测试绘制
    auto point = OH_Drawing_PointCreate(120, 120);
    OH_Drawing_CanvasDrawCircle(canvas, point, 100.0);
    OH_Drawing_PointDestroy(point);
    OH_Drawing_CanvasDetachBrush(canvas);

    if (mContent == nullptr || mContent->view == nullptr) {
        DLOG(ERROR) << "drawReactImageView mContent or view is nullptr";
        return;
    }
    
//     int32_t viewTag = mContent->view->getTag();
//     DLOG(INFO) << "drawReactImageView view tag: " << static_cast<int>(viewTag);
//     const facebook::jsi::Value value = facebook::jsi::Value(viewTag);
//     mTurboModule->call(*SharedElementManager::getInstance().runtime, "getSnap", &value, 1);
};
void RNSharedElementDrawable::drawImageView(OH_Drawing_Canvas *canvas) {
    // 测试绘制
    OH_Drawing_Path *path = OH_Drawing_PathCreate();
    constexpr int height = 300;
    constexpr int width = 300;
    constexpr float arc = 18.0f;
    int len = height / 4;
    float aX = width / 2;
    float aY = height / 4;
    float dX = aX - len * std::sin(arc);
    float dY = aY + len * std::cos(arc);
    float cX = aX + len * std::sin(arc);
    float cY = dY;
    float bX = aX + (len / 2.0);
    float bY = aY + std::sqrt((cX - dX) * (cX - dX) + (len / 2.0) * (len / 2.0));
    float eX = aX - (len / 2.0);
    float eY = bY;
    OH_Drawing_PathMoveTo(path, aX, aY);
    OH_Drawing_PathLineTo(path, bX, bY);
    OH_Drawing_PathLineTo(path, cX, cY);
    OH_Drawing_PathLineTo(path, dX, dY);
    OH_Drawing_PathLineTo(path, eX, eY);
    OH_Drawing_PathClose(path);
    OH_Drawing_CanvasDrawPath(canvas, path);
    OH_Drawing_PathDestroy(path);
};
void RNSharedElementDrawable::drawPlainView(OH_Drawing_Canvas *canvas) {
    // 测试绘制
    OH_Drawing_Rect *rect = OH_Drawing_RectCreate(100, 200, 500, 300);
    OH_Drawing_CanvasSave(nullptr);
    OH_Drawing_CanvasSave(canvas);
    OH_Drawing_CanvasTranslate(nullptr, 100, 100);
    OH_Drawing_CanvasTranslate(canvas, 100, 100);
    OH_Drawing_CanvasDrawArc(nullptr, rect, 10, 200);
    OH_Drawing_CanvasDrawArc(canvas, nullptr, 10, 200);
    OH_Drawing_CanvasDrawArc(canvas, rect, 10, 200);
    OH_Drawing_CanvasRestore(nullptr);
    OH_Drawing_CanvasRestore(canvas);
    OH_Drawing_CanvasTranslate(canvas, 200, 200);
    OH_Drawing_CanvasDrawOval(nullptr, rect);
    OH_Drawing_CanvasDrawOval(canvas, nullptr);
    OH_Drawing_CanvasDrawOval(canvas, rect);
    OH_Drawing_RectDestroy(nullptr);
    OH_Drawing_RectDestroy(rect);
};
void RNSharedElementDrawable::drawGenericView(OH_Drawing_Canvas *canvas) {
    // 测试绘制
    OH_Drawing_CanvasSave(canvas);
    OH_Drawing_CanvasTranslate(canvas, 300, 300);
    OH_Drawing_CanvasSave(canvas);
    OH_Drawing_CanvasTranslate(canvas, 100, 100);
    OH_Drawing_CanvasSave(canvas);
    OH_Drawing_CanvasTranslate(canvas, 100, 100);
    OH_Drawing_Rect *rect = OH_Drawing_RectCreate(0, 0, 100, 100);
    OH_Drawing_CanvasDrawRect(canvas, rect);
    int count = OH_Drawing_CanvasGetSaveCount(canvas);

    OH_Drawing_CanvasRestoreToCount(nullptr, count - 2);
    OH_Drawing_CanvasRestoreToCount(canvas, count - 2);

    OH_Drawing_RoundRect *roundRect = OH_Drawing_RoundRectCreate(nullptr, 20, 20);

    roundRect = OH_Drawing_RoundRectCreate(rect, 20, 20);

    OH_Drawing_CanvasDrawRoundRect(nullptr, roundRect);
    OH_Drawing_CanvasDrawRoundRect(canvas, nullptr);
    OH_Drawing_CanvasDrawRoundRect(canvas, roundRect);
    OH_Drawing_CanvasRestoreToCount(canvas, 1);
    OH_Drawing_CanvasDrawRect(canvas, rect);
    OH_Drawing_RoundRectDestroy(roundRect);
    OH_Drawing_RectDestroy(rect);
    OH_Drawing_RoundRectDestroy(nullptr);
    OH_Drawing_RectDestroy(nullptr);
};

}; // namespace rnoh