/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

#include "RNTransitionNode.h"

namespace rnoh {

RNTransitionNode::RNTransitionNode()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_CUSTOM)) {
}

RNTransitionNode::~RNTransitionNode() {}


} // namespace rnoh