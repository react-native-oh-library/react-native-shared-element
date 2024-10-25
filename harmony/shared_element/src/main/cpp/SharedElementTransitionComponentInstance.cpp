#include "SharedElementTransitionComponentInstance.h"
#include "RNOH/RNInstanceCAPI.h"
#include "SharedElementManager.h"
#include "SharedElementTurbo.h"
#include "SharedElementTransitionComponentDescriptor.h"

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
    // 从0到1的增量变化
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
            
            auto tempData = facebook::react::SharedElementTransitionEventEmitter::EventData();
            if (item == 0) {
                tempData.node = "startNode";
                // tempData.layout.x = (transitionNode.startImageStyle->offset.x) / pixel;
                // tempData.layout.y = (transitionNode.startImageStyle->offset.y) / pixel;

                tempData.layout.x = tempData.layout.visibleX = tempData.layout.contentX =
                    (transitionNode.startImageStyle->parentBoundingBox.size.width -
                     transitionNode.startImageStyle->boundingBox.size.width) /
                    2;
                tempData.layout.y = tempData.layout.visibleY = tempData.layout.contentY =
                    (transitionNode.startImageStyle->parentBoundingBox.size.height -
                     transitionNode.startImageStyle->boundingBox.size.height) /
                    2;

                tempData.layout.height = transitionNode.startImageStyle->boundingBox.size.height;
                tempData.layout.width = transitionNode.startImageStyle->boundingBox.size.width;
                // tempData.layout.visibleX = (transitionNode.startImageStyle->offset.x) / pixel;
                // tempData.layout.visibleY = (transitionNode.startImageStyle->offset.y) / pixel;
                tempData.layout.visibleWidth = transitionNode.startImageStyle->boundingBox.size.width;
                tempData.layout.visibleHeight = transitionNode.startImageStyle->boundingBox.size.height;
                // tempData.layout.contentX = (transitionNode.startImageStyle->offset.x) / pixel;
                // tempData.layout.contentY = (transitionNode.startImageStyle->offset.y) / pixel;
                tempData.layout.contentWidth = transitionNode.startImageStyle->boundingBox.size.width;
                tempData.layout.contentHeight = transitionNode.startImageStyle->boundingBox.size.height;
                if (transitionNode.startImageStyle->borderRadius.size > 0 &&
                    transitionNode.startImageStyle->borderRadius.value) {
                    tempData.style.borderTopLeftRadius = transitionNode.startImageStyle->borderRadius.value[0].f32;
                    tempData.style.borderTopRightRadius = transitionNode.startImageStyle->borderRadius.value[1].f32;
                    tempData.style.borderBottomLeftRadius = transitionNode.startImageStyle->borderRadius.value[2].f32;
                    tempData.style.borderBottomRightRadius = transitionNode.startImageStyle->borderRadius.value[3].f32;
                }
            } else {
                tempData.node = "endNode";
                
                tempData.layout.x = tempData.layout.visibleX = tempData.layout.contentX =
                    (transitionNode.endImageStyle->parentBoundingBox.size.width -
                     transitionNode.endImageStyle->boundingBox.size.width) /
                    2;
                tempData.layout.y = tempData.layout.visibleY = tempData.layout.contentY =
                    (transitionNode.endImageStyle->parentBoundingBox.size.height -
                     transitionNode.endImageStyle->boundingBox.size.height) /
                    2;

                // tempData.layout.x = (transitionNode.endImageStyle->offset.x) / pixel;
                // tempData.layout.y = (transitionNode.endImageStyle->offset.y) / pixel;
                tempData.layout.height = transitionNode.endImageStyle->boundingBox.size.height;
                tempData.layout.width = transitionNode.endImageStyle->boundingBox.size.width;
                // tempData.layout.visibleX = (transitionNode.endImageStyle->offset.x) / pixel;
                // tempData.layout.visibleY = (transitionNode.endImageStyle->offset.y) / pixel;
                tempData.layout.visibleWidth = transitionNode.endImageStyle->boundingBox.size.width;
                tempData.layout.visibleHeight = transitionNode.endImageStyle->boundingBox.size.height;
                // tempData.layout.contentX = (transitionNode.endImageStyle->offset.x) / pixel;
                // tempData.layout.contentY = (transitionNode.endImageStyle->offset.y) / pixel;
                tempData.layout.contentWidth = transitionNode.endImageStyle->boundingBox.size.width;
                tempData.layout.contentHeight = transitionNode.endImageStyle->boundingBox.size.height;

                if (transitionNode.endImageStyle->borderRadius.size > 0 &&
                    transitionNode.endImageStyle->borderRadius.value) {
                    tempData.style.borderTopLeftRadius = transitionNode.endImageStyle->borderRadius.value[0].f32;
                    tempData.style.borderTopRightRadius = transitionNode.endImageStyle->borderRadius.value[1].f32;
                    tempData.style.borderBottomLeftRadius = transitionNode.endImageStyle->borderRadius.value[2].f32;
                    tempData.style.borderBottomRightRadius = transitionNode.endImageStyle->borderRadius.value[3].f32;
                }
            }

            tempData.contentType = "image";

            m_eventEmitter->onMeasureNode(tempData);
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
