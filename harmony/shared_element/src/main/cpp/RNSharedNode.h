/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

#include "RNOH/arkui/ArkUINode.h"
#include "RNOH/arkui/NativeNodeApi.h"

namespace rnoh {
class RNSharedNode : public ArkUINode {
public:
    RNSharedNode() : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_STACK)) {}

    ~RNSharedNode() override {}

    void setUp(int item) {
        if (item == 0) {
            facebook::react::SharedColor sharedColor(0xFFFF0000);
            ArkUI_NumberValue value[] = {{.i32 = sharedColor}};
            ArkUI_AttributeItem item = {.value = value, .size = 1};
            maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_BACKGROUND_COLOR, &item));
        } else {
            facebook::react::SharedColor sharedColor(0xFFFF00FF);
            ArkUI_NumberValue value[] = {{.i32 = sharedColor}};
            ArkUI_AttributeItem item = {.value = value, .size = 1};
            maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_BACKGROUND_COLOR, &item));
        }
        ArkUI_NumberValue value[] = {{.i32 = 300}};
        ArkUI_AttributeItem sizeItem = {.value = value, .size = 1};
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_WIDTH, &sizeItem));
        maybeThrow(NativeNodeApi::getInstance()->setAttribute(m_nodeHandle, NODE_HEIGHT, &sizeItem));
    }
};
} // namespace rnoh
