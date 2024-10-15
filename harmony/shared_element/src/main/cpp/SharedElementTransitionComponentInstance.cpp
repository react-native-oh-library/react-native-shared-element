#include "SharedElementTransitionComponentInstance.h"
#include "RNOH/RNInstanceCAPI.h"
#include "SharedElementManager.h"
#include "SharedElementTurbo.h"

namespace rnoh {

SharedElementTransitionComponentInstance::SharedElementTransitionComponentInstance(Context context)
    : CppComponentInstance(std::move(context)) {
    transitionNode.parent = getParent();
}

void SharedElementTransitionComponentInstance::onChildInserted(ComponentInstance::Shared const &childComponentInstance,
                                                               std::size_t index) {
    CppComponentInstance::onChildInserted(childComponentInstance, index);
}

void SharedElementTransitionComponentInstance::onChildRemoved(ComponentInstance::Shared const &childComponentInstance) {
    CppComponentInstance::onChildRemoved(childComponentInstance);
}

void SharedElementTransitionComponentInstance::onPropsChanged(SharedConcreteProps const &props) {
    CppComponentInstance::onPropsChanged(props);
    transitionNode.setAnimation(props->animation);
    transitionNode.setAlign(props->align);
    transitionNode.setResize(props->resize);
    //从0到1的增量变化
    transitionNode.setNodePosition(props->nodePosition);
    
    if (validatePosition) {
        if (props->nodePosition > 0.9) {
            direct = 1;
        } else {
            direct = 2;
        }
        validatePosition = false;
    }
    
    int propsStartNode = props->startNode.node.nodeHandle;
    if (propsStartNode != 0 && propsStartNode != startHandle) {
        startHandle = propsStartNode;
        setViewItem(RNSharedElementTransition::START, props->startNode);
    }
    int propsEndNode = props->endNode.node.nodeHandle;
    if (propsEndNode != 0 && propsEndNode != endHandle) {
        endHandle = propsEndNode;
        setViewItem(RNSharedElementTransition::END, props->endNode);
    }
}

ArkUINode &SharedElementTransitionComponentInstance::getLocalRootArkUINode() { return transitionNode; }

void SharedElementTransitionComponentInstance::setViewItem(RNSharedElementTransition::Item item,
                                                           facebook::react::SharedElementWarpNodeStruct nodeData) {
    auto instance = m_deps.get()->rnInstance.lock();
    if (instance) {
        auto instanceCAPI = std::dynamic_pointer_cast<RNInstanceCAPI>(instance);
        if (instanceCAPI) {
            auto nodeView = instanceCAPI->findComponentInstanceByTag(nodeData.node.nodeHandle);
            auto ancestorView = instanceCAPI->findComponentInstanceByTag(nodeData.ancestor.nodeHandle);
            transitionNode.beforeTransition(static_cast<int>(item), nodeView, ancestorView);
            
            transitionNode.setItemNode(static_cast<int>(item), nodeData);
        }
    }
}

void SharedElementTransitionComponentInstance::onFinalizeUpdates() {
    
    if (!validatePosition && direct == 0) {
        validatePosition = true;
    }
    if (direct != 0) {
        transitionNode.onLayout(direct);    
    }
}

void SharedElementTransitionComponentInstance::onLayoutChanged(facebook::react::LayoutMetrics const &layoutMetrics) {
}

} // namespace rnoh
