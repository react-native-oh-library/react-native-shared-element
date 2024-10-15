#pragma once

#include "RNOH/arkui/ArkUINode.h"
#include "RNOH/arkui/TextNode.h"
#include "SharedElementTransitionComponentDescriptor.h"
#include "arkui/native_node.h"
#include "RNOH/arkui/NativeNodeApi.h"
#include "RNOH/ComponentInstance.h"
#include "RNOH/CppComponentInstance.h"
#include "RNOHCorePackage/ComponentInstances/ImageComponentInstance.h"

#include "RNSharedStyle.h"

#include "RNSharedNode.h"


namespace rnoh {
class RNTransitionNode : public ArkUINode {
public:
    RNTransitionNode();
    ~RNTransitionNode() override;

    void setAnimation(int animation) {
        if (animation_ != animation) {
            animation_ = animation;
        }
    }

    void setResize(int resize) {
        if (resize_ != resize) {
            resize_ = resize;
        }
    }

    void setAlign(int align) {
        if (align_ != align) {
            align_ = align;
        }
    }

    void setNodePosition(float nodePosition) {
        if (nodePosition_ != nodePosition) {
            nodePosition_ = nodePosition;
            mInitialNodePositionSet = true;
        }
    }

    void beforeTransition(int item, std::shared_ptr<ComponentInstance> node,
                          std::shared_ptr<ComponentInstance> ancestor) {
        if (node == nullptr) {
            return;
        }
        node->getLocalRootArkUINode().setOpacity(0);
        transitionNodes.push_back(node);


        if (item == 0) {
            startNode = node;
            startImageStyle = std::make_shared<RNSharedStyle>();
            std::shared_ptr<ComponentInstance> sourceNode = resolveNode(node, startImageStyle);
            startSharedElementNode.setSize(
                {startImageStyle->boundingBox.size.width, startImageStyle->boundingBox.size.height});
            startSharedElementNode.setSources(startImageStyle->sourceItem.string);
        } else {
            endNode = node;

            endImageStyle = std::make_shared<RNSharedStyle>();
            std::shared_ptr<ComponentInstance> sourceNode = resolveNode(node, endImageStyle);
            endSharedElementNode.setSize(
                {endImageStyle->boundingBox.size.width, endImageStyle->boundingBox.size.height});
            endSharedElementNode.setSources(endImageStyle->sourceItem.string);


            // endSharedElementNode.setUp(item);
        }
    }

    std::shared_ptr<ComponentInstance> resolveNode(std::shared_ptr<ComponentInstance> node,
                                                   std::shared_ptr<RNSharedStyle> style) {
        if (node->getChildren().size() == 1) {
            auto child = node->getChildren()[0];
            if (child->getComponentName() == "Image") {
                auto imageNode = std::static_pointer_cast<ImageComponentInstance>(child);
                style->boundingBox = imageNode->getBoundingBox();
                auto sourceItem = NativeNodeApi::getInstance()->getAttribute(
                    imageNode->getLocalRootArkUINode().getArkUINodeHandle(), NODE_IMAGE_SRC);
                style->setSource(sourceItem->string);
                style->offset =
                    NativeNodeApi::getInstance()->getLayoutPosition(node->getLocalRootArkUINode().getArkUINodeHandle());

                ArkUI_IntOffset *intOff;
                OH_ArkUI_NodeUtils_GetLayoutPositionInScreen(node->getLocalRootArkUINode().getArkUINodeHandle(),
                                                             intOff);
                style->offset = {intOff->x, intOff->y};
                return child;
            }
        }
        return node;
    }

    void setItemNode(int item, facebook::react::SharedElementWarpNodeStruct node) {
        transitionItems.insert(transitionItems.begin() + item, node);
    }

    void onLayout(int direct) {
        if (direct == 1) {
            // 取 end
            recoverAlpha = mInitialNodePositionSet && nodePosition_ == 0;
        } else {
            // 取 start
            recoverAlpha = mInitialNodePositionSet && nodePosition_ == 1;
        }
        if (recoverAlpha) {
            for (std::shared_ptr<ComponentInstance> node : transitionNodes) {
                node->getLocalRootArkUINode().setOpacity(1);
            }
            mInitialNodeLayoutPositionSet = false;
        } else {
            if (direct == 1) {
                setLayoutPosition(direct);
                
                Float scaleW = (endImageStyle->boundingBox.size.width - startImageStyle->boundingBox.size.width) * (1 - nodePosition_);
                Float endW = endImageStyle->boundingBox.size.width - scaleW;

                Float scaleH = (endImageStyle->boundingBox.size.height - startImageStyle->boundingBox.size.height) * (1 - nodePosition_);
                Float endH = endImageStyle->boundingBox.size.height - scaleH;
                
                float scalePositionX = static_cast<float>(endImageStyle->offset.x) / pixelDensity + scaleW  / 2;
                float scalePositionY = static_cast<float>(endImageStyle->offset.y) / pixelDensity + scaleH  / 2;
                
                if (endW < startImageStyle->boundingBox.size.width) {
                    endW = startImageStyle->boundingBox.size.width;
                };
                if (endH < startImageStyle->boundingBox.size.height) {
                    endH = startImageStyle->boundingBox.size.height;
                };
                setPosition({scalePositionX,  scalePositionY});
                setSize({endW, endH});
                endSharedElementNode.setSize({endW, endH});
                maybeThrow(NativeNodeApi::getInstance()->insertChildAt(
                    m_nodeHandle, endSharedElementNode.getArkUINodeHandle(), static_cast<int32_t>(-1)));
            } else {
                Float scaleW = (endImageStyle->boundingBox.size.width - startImageStyle->boundingBox.size.width) * nodePosition_;
                Float startW = startImageStyle->boundingBox.size.width + scaleW;

                Float scaleH = (endImageStyle->boundingBox.size.height - startImageStyle->boundingBox.size.height) * nodePosition_;
                Float startH = startImageStyle->boundingBox.size.width + scaleH;
                
                float scalePositionX = static_cast<float>(startImageStyle->offset.x) / pixelDensity - scaleW / 2;
                float scalePositionY = static_cast<float>(startImageStyle->offset.y) / pixelDensity - scaleH / 2;
                
                if (startW > endImageStyle->boundingBox.size.width) {
                    startW = endImageStyle->boundingBox.size.width;
                };
                if (startH > endImageStyle->boundingBox.size.height) {
                    startH = endImageStyle->boundingBox.size.height;
                };
                setPosition({scalePositionX,  scalePositionY});
                setSize({startW, startH});
                startSharedElementNode.setSize({startW, startH});
                maybeThrow(NativeNodeApi::getInstance()->insertChildAt(
                    m_nodeHandle, startSharedElementNode.getArkUINodeHandle(), static_cast<int32_t>(-1)));
            }
        }
    };
    
    int isSet = 0;

    void setLayoutPosition(int direct) {
        if (!mInitialNodeLayoutPositionSet) {
            NativeNodeApi::getInstance()->setLayoutPosition(
                m_nodeHandle, direct == 1 ? endImageStyle->offset.x : startImageStyle->offset.x,
                direct == 1 ? endImageStyle->offset.x : startImageStyle->offset.y);
            mInitialNodeLayoutPositionSet = true;
        }
    }

    using Weak = std::weak_ptr<ComponentInstance>;
    ComponentInstance::Weak parent;

private:
    float pixelDensity = 3.25;
    int animation_;
    int resize_;
    int align_;
    float nodePosition_;
    bool mInitialNodePositionSet = false;


    std::vector<facebook::react::SharedElementWarpNodeStruct> transitionItems;

    bool mInitialNodeLayoutPositionSet = false;
    bool recoverAlpha = false;
    std::vector<std::shared_ptr<ComponentInstance>> transitionNodes;
    std::shared_ptr<ComponentInstance> startNode = nullptr;
    std::shared_ptr<ComponentInstance> endNode = nullptr;

    ImageNode startSharedElementNode{};
    std::shared_ptr<RNSharedStyle> startImageStyle = std::make_shared<RNSharedStyle>();

    ImageNode endSharedElementNode{};
    std::shared_ptr<RNSharedStyle> endImageStyle = std::make_shared<RNSharedStyle>();
};
} // namespace rnoh
