//
// Created on 2024/10/12.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".
#include "SharedElementTransitionComponentDescriptor.h"
#include <arkui/native_node.h>

namespace rnoh {
class RNSharedStyle {
public:
    RNSharedStyle() {}
    ~RNSharedStyle() {}

    facebook::react::Rect boundingBox;
    ArkUI_AttributeItem sourceItem;
    ArkUI_IntOffset offset;
    // ancestor view borderRadius
    ArkUI_AttributeItem borderRadius = {.value = {}, .size = 0};

    facebook::react::Rect parentBoundingBox;

    void setSource(const char *source) { sourceItem = {.string = source}; }

    void setBorderRadius(const ArkUI_AttributeItem *item) {
        if (item->size == 1) {
            auto radius = item->value[0].f32;
            ArkUI_NumberValue values[] = {{radius}, {radius}, {radius}, {radius}};
            borderRadius = {.value = values, .size = 4};
        } else if (item->size == 4) {
            borderRadius = {.value = item->value, .size = item->size};
        } else {
            borderRadius = {.size = 0};
        }
    }
};
} // namespace rnoh