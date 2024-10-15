#include "RNTransitionNode.h"

namespace rnoh {

RNTransitionNode::RNTransitionNode()
    : ArkUINode(NativeNodeApi::getInstance()->createNode(ArkUI_NodeType::ARKUI_NODE_CUSTOM)) {
}

RNTransitionNode::~RNTransitionNode() {}


} // namespace rnoh