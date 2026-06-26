/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

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
#include "helper/Rect.h"


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
            setAlignmentWithAlign();
        }
    }

    void setNodePosition(float nodePosition) {
        if (nodePosition_ != nodePosition) {
            nodePosition_ = nodePosition;
            mInitialNodePositionSet = true;
        }
    }
    
    using Alignment = ArkUI_Alignment;
    void setAlignmentWithAlign() {
        switch (align_) {
        case 0:
            setAlignment(Alignment::ARKUI_ALIGNMENT_CENTER);
            break;
        case 1:
            // left-top
            setAlignment(Alignment::ARKUI_ALIGNMENT_TOP_START);
            break;
        case 2:
            // left-center
            setAlignment(Alignment::ARKUI_ALIGNMENT_START);
            setAlignment(Alignment::ARKUI_ALIGNMENT_CENTER);
            break;
        case 3:
            // left-bottom
            setAlignment(Alignment::ARKUI_ALIGNMENT_BOTTOM_START);
            break;
        case 4:
            // right-top
            setAlignment(Alignment::ARKUI_ALIGNMENT_TOP_START);
            break;
        case 5:
            // right-center
            setAlignment(Alignment::ARKUI_ALIGNMENT_END);
            setAlignment(Alignment::ARKUI_ALIGNMENT_CENTER);
            break;
        case 6:
            // right-bottom
            setAlignment(Alignment::ARKUI_ALIGNMENT_BOTTOM_END);
            break;
        case 7:
            // center-top
            setAlignment(Alignment::ARKUI_ALIGNMENT_TOP);
            setAlignment(Alignment::ARKUI_ALIGNMENT_CENTER);
            break;
        case 8:
            // center-center
            setAlignment(Alignment::ARKUI_ALIGNMENT_CENTER);
            break;
        case 9:
            // center-bottom
            setAlignment(Alignment::ARKUI_ALIGNMENT_BOTTOM);
            setAlignment(Alignment::ARKUI_ALIGNMENT_CENTER);
            break;
        }
    }

    void beforeTransition(int item, std::shared_ptr<ComponentInstance> node,
                          std::shared_ptr<ComponentInstance> ancestor) {
        if (node == nullptr) {
            return;
        }

        transitionNodes.push_back(node);
        
        if (item == 0) {
            startNode = node;

            if (animation_ == 0 || animation_ == 1 || animation_ == 2 || animation_ == 3 || animation_ == 4) {
                node->getLocalRootArkUINode().setOpacity(1.0f);
            } else {
                node->getLocalRootArkUINode().setOpacity(0.0f);
            }

            startImageStyle = std::make_shared<RNSharedStyle>();
            std::shared_ptr<ComponentInstance> sourceNode = resolveNode(node, startImageStyle);
            startSharedElementNode.setSize(
                {startImageStyle->boundingBox.size.width, startImageStyle->boundingBox.size.height});
            startSharedElementNode.setSources(startImageStyle->sourceItem.string);

            auto borderRadius = NativeNodeApi::getInstance()->getAttribute(
                ancestor->getLocalRootArkUINode().getArkUINodeHandle(), NODE_BORDER_RADIUS);
            startImageStyle->setBorderRadius(borderRadius);

            startImageStyle->parentBoundingBox = ancestor->getBoundingBox();

        } else {
            endNode = node;

            if (animation_ == 0 || animation_ == 3 || animation_ == 4) {
                node->getLocalRootArkUINode().setOpacity(1.0f);
            } else {
                node->getLocalRootArkUINode().setOpacity(0.0f);
            }

            endImageStyle = std::make_shared<RNSharedStyle>();
            std::shared_ptr<ComponentInstance> sourceNode = resolveNode(node, endImageStyle);
            endSharedElementNode.setSize(
                {endImageStyle->boundingBox.size.width, endImageStyle->boundingBox.size.height});
            endSharedElementNode.setSources(endImageStyle->sourceItem.string);

            auto borderRadius = NativeNodeApi::getInstance()->getAttribute(
                ancestor->getLocalRootArkUINode().getArkUINodeHandle(), NODE_BORDER_RADIUS);
            endImageStyle->setBorderRadius(borderRadius);

            endImageStyle->parentBoundingBox = ancestor->getBoundingBox();
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

                ArkUI_IntOffset intOff = {0, 0};
                OH_ArkUI_NodeUtils_GetLayoutPositionInScreen(node->getLocalRootArkUINode().getArkUINodeHandle(),
                                                             &intOff);
                style->offset = {intOff.x, intOff.y};

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
            recoverAlpha = mInitialNodePositionSet && nodePosition_ == 0;
        } else {
            recoverAlpha = mInitialNodePositionSet && nodePosition_ == 1;
        }

        if (recoverAlpha) {
            for (std::shared_ptr<ComponentInstance> node : transitionNodes) {
                if (animation_ != 4 || node != startNode) {
                    node->getLocalRootArkUINode().setOpacity(1);
                }
            }
            mInitialNodeLayoutPositionSet = false;
        } else {
            if (direct == 1) {
                setLayoutPosition(direct);

                float startWidth = startImageStyle->boundingBox.size.width;
                float startHeight = startImageStyle->boundingBox.size.height;
                float endWidth = endImageStyle->boundingBox.size.width;
                float endHeight = endImageStyle->boundingBox.size.height;

                float startX = static_cast<float>(startImageStyle->offset.x) / pixelDensity;
                float startY = static_cast<float>(startImageStyle->offset.y) / pixelDensity;
                float endX = static_cast<float>(endImageStyle->offset.x) / pixelDensity;
                float endY = static_cast<float>(endImageStyle->offset.y) / pixelDensity;

                float currentWidth = startWidth + (endWidth - startWidth) * nodePosition_;
                float currentHeight = startHeight + (endHeight - startHeight) * nodePosition_;

                float currentX = startX + (endX - startX) * nodePosition_;
                float currentY = startY + (endY - startY) * nodePosition_;

                currentX = currentX - (currentWidth - startWidth) / 2;
                currentY = currentY - (currentHeight - startHeight) / 2;

                setPosition({currentX, currentY});
                setSize({currentWidth, currentHeight});
                endSharedElementNode.setSize({currentWidth, currentHeight});
                endSharedElementNode.setPosition({0, 0});

                applyOpacity(animation_);

                maybeThrow(NativeNodeApi::getInstance()->insertChildAt(
                    m_nodeHandle, endSharedElementNode.getArkUINodeHandle(), static_cast<int32_t>(-1)));
            } else {
                float startWidth = startImageStyle->boundingBox.size.width;
                float startHeight = startImageStyle->boundingBox.size.height;
                float endWidth = endImageStyle->boundingBox.size.width;
                float endHeight = endImageStyle->boundingBox.size.height;

                float startX = static_cast<float>(startImageStyle->offset.x) / pixelDensity;
                float startY = static_cast<float>(startImageStyle->offset.y) / pixelDensity;
                float endX = static_cast<float>(endImageStyle->offset.x) / pixelDensity;
                float endY = static_cast<float>(endImageStyle->offset.y) / pixelDensity;

                float currentWidth = startWidth + (endWidth - startWidth) * (1 - nodePosition_);
                float currentHeight = startHeight + (endHeight - startHeight) * (1 - nodePosition_);

                float currentX = startX + (endX - startX) * (1 - nodePosition_);
                float currentY = startY + (endY - startY) * (1 - nodePosition_);

                currentX = currentX - (currentWidth - startWidth) / 2;
                currentY = currentY - (currentHeight - startHeight) / 2;

                setPosition({currentX, currentY});
                setSize({currentWidth, currentHeight});
                startSharedElementNode.setSize({currentWidth, currentHeight});
                startSharedElementNode.setPosition({0, 0});

                applyOpacity(animation_);

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

    void applyOpacity(int animationType) {
        if (animationType == 1) {
            float startOpacity = 1.0f - nodePosition_;
            float endOpacity = nodePosition_;
            
            if (startNode != nullptr) {
                startNode->getLocalRootArkUINode().setOpacity(startOpacity);
            }
            if (endNode != nullptr) {
                endNode->getLocalRootArkUINode().setOpacity(endOpacity);
            }
            
            setOpacity(0.0f);
        } else if (animationType == 2) {
            if (startNode != nullptr) {
                startNode->getLocalRootArkUINode().setOpacity(1.0f);
            }
            if (endNode != nullptr) {
                endNode->getLocalRootArkUINode().setOpacity(0.0f);
            }
            
            setOpacity(0.0f);
        } else if (animationType == 3) {
            if (startNode != nullptr) {
                startNode->getLocalRootArkUINode().setOpacity(0.0f);
            }
            if (endNode != nullptr) {
                endNode->getLocalRootArkUINode().setOpacity(1.0f);
            }
            
            setOpacity(0.0f);
        } else if (animationType == 4) {
            float startOpacity = 1.0f - nodePosition_;
            
            if (startNode != nullptr) {
                startNode->getLocalRootArkUINode().setOpacity(startOpacity);
            }
            if (endNode != nullptr) {
                endNode->getLocalRootArkUINode().setOpacity(1.0f);
            }
            
            setOpacity(0.0f);
        } else {
            setOpacity(1.0);
        }
    }

    using Weak = std::weak_ptr<ComponentInstance>;
    ComponentInstance::Weak parent;
    std::shared_ptr<RNSharedStyle> startImageStyle = std::make_shared<RNSharedStyle>();
    std::shared_ptr<RNSharedStyle> endImageStyle = std::make_shared<RNSharedStyle>();

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
    ImageNode endSharedElementNode{};
};
} // namespace rnoh
