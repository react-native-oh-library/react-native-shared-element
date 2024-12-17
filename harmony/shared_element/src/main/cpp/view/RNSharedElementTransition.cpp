/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */
 
#include "RNSharedElementTransition.h"

namespace rnoh {

RNSharedElementTransition::RNSharedElementTransition()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_STACK)) {
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_ON_CLICK, 0, this));
    maybeThrow(NativeNodeApi::getInstance()->registerNodeEvent(m_nodeHandle, NODE_ON_HOVER, 0, this));
}

RNSharedElementTransition::~RNSharedElementTransition() {
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_ON_CLICK);
    NativeNodeApi::getInstance()->unregisterNodeEvent(m_nodeHandle, NODE_ON_HOVER);
}

void RNSharedElementTransition::setUp(std::shared_ptr<RNSharedElementNodeManager> nodeManager,
                                      std::shared_ptr<SharedElementTurbo> turbo) {
    nodeMgr = nodeManager;
    std::shared_ptr<RNSharedElementTransitionItem> startItem =
        std::make_shared<RNSharedElementTransitionItem>(nodeMgr, "start");
    std::shared_ptr<RNSharedElementTransitionItem> endItem =
        std::make_shared<RNSharedElementTransitionItem>(nodeMgr, "end");
    transitionItems.push_back(startItem);
    transitionItems.push_back(endItem);

    maybeThrow(NativeNodeApi::getInstance()->addChild(m_nodeHandle, mStartView.getArkUINodeHandle()));
    maybeThrow(NativeNodeApi::getInstance()->addChild(m_nodeHandle, mEndView.getArkUINodeHandle()));

    mStartView.mDrawable.mTurboModule = turbo;
    mEndView.mDrawable.mTurboModule = turbo;
};


}; // namespace rnoh
