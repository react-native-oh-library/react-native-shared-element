//
// Created on 2024/9/26.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".
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
