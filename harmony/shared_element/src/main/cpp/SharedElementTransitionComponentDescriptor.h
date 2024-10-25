#pragma once

#include <react/renderer/components/view/ConcreteViewShadowNode.h>
#include <react/renderer/components/view/ViewShadowNode.h>
#include <react/renderer/core/ConcreteComponentDescriptor.h>
#include <react/renderer/core/propsConversions.h>

namespace facebook {
namespace react {

inline const char SharedElementTransitionComponentName[] = "RNSharedElementTransition";

class SharedElementNodeStyleCls {
public:
    int resizeMode;
    std::string backgroundColor;
    std::string borderColor;
    std::string color;
};

class SharedElementNodeCls {
public:
    int nodeHandle;
    bool isParent;
    SharedElementNodeStyleCls nodeStyle;
};

struct SharedElementWarpNodeStruct {
    SharedElementNodeCls node;
    SharedElementNodeCls ancestor;
};

class SharedElementTransitionProps : public ViewProps {
public:
    SharedElementTransitionProps() = default;

    SharedElementTransitionProps(const PropsParserContext &context, const SharedElementTransitionProps &sourceProps,
                                 const RawProps &rawProps)
        : ViewProps(context, sourceProps, rawProps),
          startNode(convertRawProp(context, rawProps, "startNode", sourceProps.startNode, {})),
          endNode(convertRawProp(context, rawProps, "endNode", sourceProps.endNode, {})),
          nodePosition(convertRawProp(context, rawProps, "nodePosition", sourceProps.nodePosition, {})),
          animation(convertRawProp(context, rawProps, "animation", sourceProps.animation, {})),
          resize(convertRawProp(context, rawProps, "resize", sourceProps.resize, {})),
          align(convertRawProp(context, rawProps, "align", sourceProps.align, {})) {}

    SharedElementWarpNodeStruct startNode{};
    SharedElementWarpNodeStruct endNode{};
    float nodePosition{};
    int animation{};
    int resize{};
    int align{};
};


static inline void fromSharedElementNodeRawValue(const PropsParserContext &context,
                                                 butter::map<std::string, RawValue> map, SharedElementNodeCls &result) {
    auto tmp_nodeHandle = map.find("nodeHandle");
    if (tmp_nodeHandle != map.end()) {
        fromRawValue(context, tmp_nodeHandle->second, result.nodeHandle);
    }
    auto tmp_isParent = map.find("isParent");
    if (tmp_isParent != map.end()) {
        fromRawValue(context, tmp_isParent->second, result.isParent);
    }
    auto tmp_nodeStyle = map.find("nodeStyle");
    if (tmp_nodeStyle != map.end()) {
        auto map = (butter::map<std::string, RawValue>)tmp_nodeStyle->second;
        auto tmp_resizeMode = map.find("resizeMode");
        if (tmp_resizeMode != map.end()) {
            fromRawValue(context, tmp_resizeMode->second, result.nodeStyle.resizeMode);
        }
        auto tmp_backgroundColor = map.find("backgroundColor");
        if (tmp_backgroundColor != map.end()) {
            fromRawValue(context, tmp_backgroundColor->second, result.nodeStyle.backgroundColor);
        }
        auto tmp_borderColor = map.find("borderColor");
        if (tmp_borderColor != map.end()) {
            fromRawValue(context, tmp_borderColor->second, result.nodeStyle.borderColor);
        }
        auto tmp_color = map.find("color");
        if (tmp_color != map.end()) {
            fromRawValue(context, tmp_color->second, result.nodeStyle.color);
        }
    }
}

static inline void fromRawValue(const PropsParserContext &context, const RawValue &value,
                                SharedElementWarpNodeStruct &result) {
    auto map = (butter::map<std::string, RawValue>)value;

    auto tmp_node = map.find("node");
    if (tmp_node != map.end()) {
        fromSharedElementNodeRawValue(context, (butter::map<std::string, RawValue>)tmp_node->second, result.node);
    }
    auto tmp_ancestor = map.find("ancestor");
    if (tmp_ancestor != map.end()) {
        fromSharedElementNodeRawValue(context, (butter::map<std::string, RawValue>)tmp_ancestor->second,
                                      result.ancestor);
    }
}

class JSI_EXPORT SharedElementTransitionEventEmitter : public ViewEventEmitter {
public:
    using ViewEventEmitter::ViewEventEmitter;

    struct LayoutData {
        double x;
        double y;
        double width;
        double height;
        double visibleX;
        double visibleY;
        double visibleWidth;
        double visibleHeight;
        double contentX;
        double contentY;
        double contentWidth;
        double contentHeight;
    };

    struct StyleData {
        float borderTopLeftRadius;
        float borderTopRightRadius;
        float borderBottomLeftRadius;
        float borderBottomRightRadius;
    };

    struct EventData {
        std::string node;
        LayoutData layout;
        StyleData style;
        std::string contentType;
    };

    void onMeasureNode(EventData event) const {
        dispatchEvent("onMeasureNode", [event = std::move(event)](jsi::Runtime &runtime) {
            auto payload = jsi::Object(runtime);
            auto payloadLayout = jsi::Object(runtime);
            payloadLayout.setProperty(runtime, "x", event.layout.x);
            payloadLayout.setProperty(runtime, "y", event.layout.y);
            payloadLayout.setProperty(runtime, "width", event.layout.width);
            payloadLayout.setProperty(runtime, "height", event.layout.height);
            payloadLayout.setProperty(runtime, "visibleX", event.layout.visibleX);
            payloadLayout.setProperty(runtime, "visibleY", event.layout.visibleY);
            payloadLayout.setProperty(runtime, "visibleWidth", event.layout.visibleWidth);
            payloadLayout.setProperty(runtime, "visibleHeight", event.layout.visibleHeight);
            payloadLayout.setProperty(runtime, "contentX", event.layout.contentX);
            payloadLayout.setProperty(runtime, "contentY", event.layout.contentY);
            payloadLayout.setProperty(runtime, "contentWidth", event.layout.contentWidth);
            payloadLayout.setProperty(runtime, "visibleHeight", event.layout.visibleHeight);
            payloadLayout.setProperty(runtime, "contentHeight", event.layout.contentHeight);

            auto payloadStyle = jsi::Object(runtime);
            payloadStyle.setProperty(runtime, "borderTopLeftRadius", event.style.borderTopLeftRadius);
            payloadStyle.setProperty(runtime, "borderTopRightRadius", event.style.borderTopRightRadius);
            payloadStyle.setProperty(runtime, "borderBottomLeftRadius", event.style.borderBottomLeftRadius);
            payloadStyle.setProperty(runtime, "borderBottomRightRadius", event.style.borderBottomRightRadius);

            payload.setProperty(runtime, "contentType", event.contentType);
            payload.setProperty(runtime, "layout", payloadLayout);
            payload.setProperty(runtime, "node", event.node);
            payload.setProperty(runtime, "style", payloadStyle);

            return payload;
        });
    }
};

using SharedElementTransitionShadowNode =
    ConcreteViewShadowNode<SharedElementTransitionComponentName, SharedElementTransitionProps,
                           SharedElementTransitionEventEmitter>;

class SharedElementTransitionComponentDescriptor final
    : public ConcreteComponentDescriptor<SharedElementTransitionShadowNode> {
public:
    SharedElementTransitionComponentDescriptor(ComponentDescriptorParameters const &parameters)
        : ConcreteComponentDescriptor(parameters) {}
};

} // namespace react
} // namespace facebook
