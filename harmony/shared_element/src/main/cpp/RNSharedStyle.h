/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */
 
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