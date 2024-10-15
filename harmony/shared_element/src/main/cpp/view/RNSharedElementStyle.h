#pragma once

#include "helper/Rect.h"
#include "helper/Matrix.h"
#include "helper/InterpolatingScaleType.h"

#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/ArkUINode.h"
#include "SharedElementTransitionComponentDescriptor.h"

namespace rnoh {

class RNSharedElementStyle {
public:
    // static drawing::Rect EMPTY_RECT;
    // static drawing::Rect EMPTY_RECTF;
    static const int PROP_OPACITY = 1 << 0;
    static const int PROP_ELEVATION = 1 << 1;
    static const int PROP_BACKGROUND_COLOR = 1 << 2;
    static const int PROP_BORDER_COLOR = 1 << 3;
    static const int PROP_BORDER_WIDTH = 1 << 4;
    static const int PROP_BORDER_STYLE = 1 << 5;
    static const int PROP_BORDER_TOPLEFTRADIUS = 1 << 6;
    static const int PROP_BORDER_TOPRIGHTRADIUS = 1 << 7;
    static const int PROP_BORDER_BOTTOMLEFTRADIUS = 1 << 8;
    static const int PROP_BORDER_BOTTOMRIGHT_RADIUS = 1 << 9;
    static const int PROP_BORDER = PROP_BORDER_COLOR + PROP_BORDER_WIDTH + PROP_BORDER_STYLE +
                                   PROP_BORDER_TOPLEFTRADIUS + PROP_BORDER_TOPRIGHTRADIUS +
                                   PROP_BORDER_BOTTOMLEFTRADIUS + PROP_BORDER_BOTTOMRIGHT_RADIUS;
    static const int PROP_SCALETYPE = 1 << 10;

    // 成员变量
    std::shared_ptr<drawing::Rect> layout = std::make_shared<drawing::Rect>();         // 绝对布局
    std::shared_ptr<drawing::Rect> frame = std::make_shared<drawing::Rect>();          // 相对于父级的框架
    std::shared_ptr<drawing::Rect> ancestorLayout = std::make_shared<drawing::Rect>(); // 祖先的绝对布局
    std::shared_ptr<drawing::Matrix> ancestorTransform = std::make_shared<drawing::Matrix>();
    ScaleType scaleType = ScaleType::FIT_XY;
    int backgroundColor = 0XFFFFFFFF;
    float opacity = 1;
    float borderTopLeftRadius;
    float borderTopRightRadius;
    float borderBottomLeftRadius;
    float borderBottomRightRadius;
    float borderWidth;
    int borderColor = 0XFFFFFFFF;
    std::string borderStyle = "solid";
    float elevation;

    // facebook::react::SharedElementNodeStyleCls mConfig;

    RNSharedElementStyle(){};
    RNSharedElementStyle(facebook::react::SharedElementNodeStyleCls config){
        // todo
        /*if (config.hasKey("opacity"))
            opacity = (float)config.getDouble("opacity");
        if (config.hasKey("backgroundColor"))
            backgroundColor = config.getInt("backgroundColor");
        if (config.hasKey("borderColor"))
            borderColor = config.getInt("borderColor");
        if (config.hasKey("borderWidth"))
            borderWidth = PixelUtil.toPixelFromDIP((float)config.getDouble("borderWidth"));
        if (config.hasKey("borderStyle"))
            borderStyle = config.getString("borderStyle");
        if (config.hasKey("resizeMode"))
            scaleType = ImageResizeMode.toScaleType(config.getString("resizeMode"));
        if (config.hasKey("elevation"))
            elevation = PixelUtil.toPixelFromDIP((float)config.getDouble("elevation"));

        // Border-radius
        boolean isRTL = I18nUtil.getInstance().isRTL(context);
        if (config.hasKey("borderRadius")) {
            float borderRadius = PixelUtil.toPixelFromDIP((float)config.getDouble("borderRadius"));
            borderTopLeftRadius = borderRadius;
            borderTopRightRadius = borderRadius;
            borderBottomLeftRadius = borderRadius;
            borderBottomRightRadius = borderRadius;
        }
        if (config.hasKey("borderTopEndRadius")) {
            float borderRadius = PixelUtil.toPixelFromDIP((float)config.getDouble("borderTopEndRadius"));
            if (isRTL) {
                borderTopLeftRadius = borderRadius;
            } else {
                borderTopRightRadius = borderRadius;
            }
        }
        if (config.hasKey("borderTopStartRadius")) {
            float borderRadius = PixelUtil.toPixelFromDIP((float)config.getDouble("borderTopStartRadius"));
            if (isRTL) {
                borderTopRightRadius = borderRadius;
            } else {
                borderTopLeftRadius = borderRadius;
            }
        }
        if (config.hasKey("borderBottomEndRadius")) {
            float borderRadius = PixelUtil.toPixelFromDIP((float)config.getDouble("borderBottomEndRadius"));
            if (isRTL) {
                borderBottomLeftRadius = borderRadius;
            } else {
                borderBottomRightRadius = borderRadius;
            }
        }
        if (config.hasKey("borderBottomStartRadius")) {
            float borderRadius = PixelUtil.toPixelFromDIP((float)config.getDouble("borderBottomStartRadius"));
            if (isRTL) {
                borderBottomRightRadius = borderRadius;
            } else {
                borderBottomLeftRadius = borderRadius;
            }
        }
        if (config.hasKey("borderTopLeftRadius"))
            borderTopLeftRadius = PixelUtil.toPixelFromDIP((float)config.getDouble("borderTopLeftRadius"));
        if (config.hasKey("borderTopRightRadius"))
            borderTopRightRadius = PixelUtil.toPixelFromDIP((float)config.getDouble("borderTopRightRadius"));
        if (config.hasKey("borderBottomLeftRadius"))
            borderBottomLeftRadius = PixelUtil.toPixelFromDIP((float)config.getDouble("borderBottomLeftRadius"));
        if (config.hasKey("borderBottomRightRadius"))
            borderBottomRightRadius = PixelUtil.toPixelFromDIP((float)config.getDouble("borderBottomRightRadius"));*/
    };

    int compare(const std::shared_ptr<RNSharedElementStyle> style) const {
        int res = 0;
        if (opacity != style->opacity)
            res += PROP_OPACITY;
        if (backgroundColor != style->backgroundColor)
            res += PROP_BACKGROUND_COLOR;
        if (borderColor != style->borderColor)
            res += PROP_BORDER_COLOR;
        if (borderWidth != style->borderWidth)
            res += PROP_BORDER_WIDTH;
        if (borderStyle != style->borderStyle)
            res += PROP_BORDER_STYLE;
        if (borderTopLeftRadius != style->borderTopLeftRadius)
            res += PROP_BORDER_TOPLEFTRADIUS;
        if (borderTopRightRadius != style->borderTopRightRadius)
            res += PROP_BORDER_TOPRIGHTRADIUS;
        if (borderBottomLeftRadius != style->borderBottomLeftRadius)
            res += PROP_BORDER_BOTTOMLEFTRADIUS;
        if (borderBottomRightRadius != style->borderBottomRightRadius)
            res += PROP_BORDER_BOTTOMRIGHT_RADIUS;
        if (elevation != style->elevation)
            res += PROP_ELEVATION;
        if (!equalsScaleType(scaleType, style->scaleType))
            res += PROP_SCALETYPE;
        return res;
    }

    bool isVisible() const {
        if (opacity <= 0)
            return false;
        if (elevation > 0)
            return true;
        return (getAlpha(backgroundColor) > 0) || (getAlpha(borderColor) > 0);
    }

    static int getAlpha(int color) { return (color >> 24) & 0xFF; }

    static std::shared_ptr<drawing::Rect> normalizeLayout(bool compensateForTransforms,
                                         const std::shared_ptr<RNSharedElementStyle> style,
                                         std::vector<int> parentOffset) {
        std::shared_ptr<drawing::Rect> emptyRect = std::make_shared<drawing::Rect>();
        if (style == nullptr)
            return emptyRect;

        return RNSharedElementStyle::normalizeLayout(compensateForTransforms, style->layout, style, parentOffset);
    }

    static std::shared_ptr<drawing::Rect> normalizeLayout(bool compensateForTransforms, const std::shared_ptr<drawing::Rect> layout,
                                         const std::shared_ptr<RNSharedElementStyle> style,
                                         std::vector<int> parentOffset) {
        if (compensateForTransforms && style != nullptr) {
            std::shared_ptr<drawing::Rect> normalizedLayout = layout;
            normalizedLayout->Offset(-parentOffset[0], -parentOffset[1]);
            auto transform = style->ancestorTransform->Invert().value();
            transform.MapRect(*normalizedLayout);
            normalizedLayout->Offset(parentOffset[0], parentOffset[1]);
            return normalizedLayout;
        }
        return layout;
    };

    static bool equalsScaleType(ScaleType scaleType1, ScaleType scaleType2) { return scaleType1 == scaleType2; };

    static ScaleType getInterpolatingScaleType(const std::shared_ptr<RNSharedElementStyle> style1,
                                               const std::shared_ptr<drawing::Rect> layout1,
                                               const std::shared_ptr<RNSharedElementStyle> style2,
                                               const std::shared_ptr<drawing::Rect> layout2, float position) {
        if (style1->scaleType == style2->scaleType) {
            return style1->scaleType;
        }
        std::shared_ptr<drawing::Rect> boundsFrom = layout1;
        std::shared_ptr<drawing::Rect> boundsTo = layout2;
        std::shared_ptr<InterpolatingScaleType> scaleType = std::make_shared<InterpolatingScaleType>(
            style1->scaleType, style2->scaleType,
            std::make_shared<drawing::Rect>(0, 0, static_cast<int>(boundsFrom->GetWidth()), static_cast<int>(boundsFrom->GetHeight())),
            std::make_shared<drawing::Rect>(0, 0, static_cast<int>(boundsTo->GetWidth()), static_cast<int>(boundsTo->GetHeight())));
        scaleType->setValue(position);
        return scaleType->getScaleType();
    };

    static std::shared_ptr<drawing::Rect> getInterpolatedLayout(const std::shared_ptr<drawing::Rect> layout1, const std::shared_ptr<drawing::Rect> layout2,
                                               float position) {
        std::shared_ptr<drawing::Rect> bounds1 = layout1;
        std::shared_ptr<drawing::Rect> bounds2 = layout2;
        return std::make_shared<drawing::Rect>(bounds1->GetLeft() + ((bounds2->GetLeft() - bounds1->GetLeft()) * position),
                             bounds1->GetTop() + ((bounds2->GetTop() - bounds1->GetTop()) * position),
                             bounds1->GetRight() + ((bounds2->GetRight() - bounds1->GetRight()) * position),
                             bounds1->GetBottom() + ((bounds2->GetBottom() - bounds1->GetBottom()) * position));
    }

    static int getInterpolatedColor(int color1, int color2, float position) {
        int alphaA = (color1 >> 24) & 0xFF;
        int redA = (color1 >> 16) & 0xFF;
        int greenA = (color1 >> 8) & 0xFF;
        int blueA = color1 & 0xFF;

        int alphaB = (color2 >> 24) & 0xFF;
        int redB = (color2 >> 16) & 0xFF;
        int greenB = (color2 >> 8) & 0xFF;
        int blueB = color2 & 0xFF;

        int alpha = static_cast<int>(alphaA + ((alphaB - alphaA) * position));
        int red = static_cast<int>(redA + ((redB - redA) * position));
        int green = static_cast<int>(greenA + ((greenB - greenA) * position));
        int blue = static_cast<int>(blueA + ((blueB - blueA) * position));

        return (alpha << 24) | (red << 16) | (green << 8) | blue;
    }

    static std::shared_ptr<RNSharedElementStyle>
    getInterpolatedStyle(const std::shared_ptr<RNSharedElementStyle> style1, const std::shared_ptr<drawing::Rect> layout1,
                         const std::shared_ptr<RNSharedElementStyle> style2, const std::shared_ptr<drawing::Rect> layout2,
                         float position) {

        std::shared_ptr<RNSharedElementStyle> result = std::make_shared<RNSharedElementStyle>();

        result->scaleType = getInterpolatingScaleType(style1, layout1, style2, layout2, position);
        result->opacity = style1->opacity + ((style2->opacity - style1->opacity) * position);
        result->backgroundColor = getInterpolatedColor(style1->backgroundColor, style2->backgroundColor, position);

        result->borderTopLeftRadius =
            style1->borderTopLeftRadius + ((style2->borderTopLeftRadius - style1->borderTopLeftRadius) * position);
        result->borderTopRightRadius =
            style1->borderTopRightRadius + ((style2->borderTopRightRadius - style1->borderTopRightRadius) * position);
        result->borderBottomLeftRadius = style1->borderBottomLeftRadius +
                                         ((style2->borderBottomLeftRadius - style1->borderBottomLeftRadius) * position);
        result->borderBottomRightRadius =
            style1->borderBottomRightRadius +
            ((style2->borderBottomRightRadius - style1->borderBottomRightRadius) * position);
        result->borderWidth = style1->borderWidth + ((style2->borderWidth - style1->borderWidth) * position);
        result->borderColor = getInterpolatedColor(style1->borderColor, style2->borderColor, position);
        result->elevation = style1->elevation + ((style2->elevation - style1->elevation) * position);

        return result;
    }

    static std::shared_ptr<drawing::Matrix> getAbsoluteViewTransform(std::shared_ptr<ComponentInstance> &view) {
        // todo
        //Matrix matrix = new Matrix(view.getMatrix());
        std::shared_ptr<drawing::Matrix> matrix = std::make_shared<drawing::Matrix>();
        float vals[9];
        //matrix.getValues(vals);
        if (view == nullptr) {
            DLOG(WARNING) << "getAbsoluteViewTransform view is nullptr";
            return matrix;
        }
        float vals2[9];
        std::shared_ptr<ComponentInstance> parentView = view->getParent().lock();

        while (parentView) {
            //Matrix parentMatrix = ((View) parentView).getMatrix();
            //parentMatrix.getValues(vals2);
            // Matrix transformation
            vals[0] *= vals2[0]; // MSCALE_X
            vals[1] *= vals2[1]; // MSKEW_X
            vals[2] += vals2[2]; // MTRANS_X
            vals[3] *= vals2[3]; // MSKEW_Y
            vals[4] *= vals2[4]; // MSCALE_Y
            vals[5] += vals2[5]; // MTRANS_Y

            parentView = parentView->getParent().lock();
        }
        matrix->SetMatrix(vals[0], vals[1], vals[2], vals[3], vals[4], vals[5], 0, 0, 0);
        return matrix;
    }

    static float getAncestorVisibility(std::weak_ptr<ComponentInstance> view,
                                       const std::shared_ptr<RNSharedElementStyle> style) {
        std::shared_ptr<drawing::Rect> rect = std::make_shared<drawing::Rect>();
        RNSharedElementStyle::getLayoutOnScreen(view, rect);
        std::shared_ptr<drawing::Rect> intersection = std::make_shared<drawing::Rect>();
        //fixme if (!intersection.setIntersect(rect, style.ancestorLayout)) return 0.0f; ?
        if (!intersection->Intersect(*style->ancestorLayout)) {
            return 0.0f;
        }
        float superVolume = rect->GetWidth() * rect->GetHeight();
        float intersectionVolume = intersection->GetWidth() * intersection->GetHeight();
        float ancestorVolume = style->ancestorLayout->GetWidth() * style->ancestorLayout->GetHeight();
        return (intersectionVolume / superVolume) * (intersectionVolume / ancestorVolume);
    }

    static void getLayoutOnScreen(std::weak_ptr<ComponentInstance> view, std::shared_ptr<drawing::Rect> output) {
        auto viewComponent = view.lock();
        if (viewComponent == nullptr) {
            return;
        }
        std::shared_ptr<drawing::Rect> tmp = std::make_shared<drawing::Rect>(0.0f, 0.0f, viewComponent->getLocalRootArkUINode().getSavedWidth(),
                          viewComponent->getLocalRootArkUINode().getSavedHeight());
        output = tmp;
        // todo
        /*
        output.set(0.0f, 0.0f, view.getWidth(), view.getHeight());
         view.getMatrix().mapRect(output);
         output.left += view.getLeft();
         output.top += view.getTop();

         View* viewParent = view.getParent();
         while (viewParent) {
             output.left -= viewParent->getScrollX();
             output.top -= viewParent->getScrollY();
             viewParent->getMatrix().mapRect(output);
             output.left += viewParent->getLeft();
             output.top += viewParent->getTop();

             viewParent = viewParent->getParent();
         }
        */
    };
};
}; // namespace rnoh
