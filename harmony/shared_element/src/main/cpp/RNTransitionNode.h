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

        node->getLocalRootArkUINode().setOpacity(0);

        transitionNodes.push_back(node);
        
        if (item == 0) {
            startNode = node;

            if (animation_ == 2) {
                startNode->getLocalRootArkUINode().setOpacity(1);
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

            if (animation_ == 3) {
                endNode->getLocalRootArkUINode().setOpacity(1);
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

                if (endImageStyle->boundingBox.size.width > startImageStyle->boundingBox.size.width &&
                    endImageStyle->boundingBox.size.height > startImageStyle->boundingBox.size.height) {
                    // endImageStyle 矩形在宽度和高度上都更大
                    facebook::react::Float scaleW = (endImageStyle->boundingBox.size.width - startImageStyle->boundingBox.size.width) *
                                   (1 - nodePosition_);
                    facebook::react::Float endW = endImageStyle->boundingBox.size.width - scaleW;

                    facebook::react::Float scaleH = (endImageStyle->boundingBox.size.height - startImageStyle->boundingBox.size.height) *
                                   (1 - nodePosition_);
                    facebook::react::Float endH = endImageStyle->boundingBox.size.height - scaleH;

                    float scalePositionX = static_cast<float>(endImageStyle->offset.x) / pixelDensity + scaleW / 2;
                    float scalePositionY = static_cast<float>(endImageStyle->offset.y) / pixelDensity + scaleH / 2;

                    if (endW < startImageStyle->boundingBox.size.width) {
                        endW = startImageStyle->boundingBox.size.width;
                    };
                    if (endH < startImageStyle->boundingBox.size.height) {
                        endH = startImageStyle->boundingBox.size.height;
                    };
                    setPosition({scalePositionX, scalePositionY});
                    setSize({endW, endH});
                    endSharedElementNode.setSize({endW, endH});
                } else if (endImageStyle->boundingBox.size.width < startImageStyle->boundingBox.size.width &&
                           endImageStyle->boundingBox.size.height < startImageStyle->boundingBox.size.height) {
                    // startImageStyle 矩形在宽度和高度上都更大
                    // 计算宽度和高度的缩放量
                    facebook::react::Float scaleW = (startImageStyle->boundingBox.size.width - endImageStyle->boundingBox.size.width) *
                                   nodePosition_;
                    facebook::react::Float endW = startImageStyle->boundingBox.size.width - scaleW;

                    facebook::react::Float scaleH = (startImageStyle->boundingBox.size.height - endImageStyle->boundingBox.size.height) *
                                   nodePosition_;
                    facebook::react::Float endH = startImageStyle->boundingBox.size.height - scaleH;

                    // 计算缩放后的位置
                    float scalePositionX = static_cast<float>(startImageStyle->offset.x) / pixelDensity + scaleW / 2;
                    float scalePositionY = static_cast<float>(startImageStyle->offset.y) / pixelDensity + scaleH / 2;

                    // 确保宽度和高度不小于 endImageStyle 的最小值
                    if (endW > startImageStyle->boundingBox.size.width) {
                        endW = startImageStyle->boundingBox.size.width;
                    }
                    if (endH > startImageStyle->boundingBox.size.height) {
                        endH = startImageStyle->boundingBox.size.height;
                    }

                    // 设置位置和大小
                    setPosition({scalePositionX, scalePositionY});
                    setSize({endW, endH});
                    endSharedElementNode.setSize({endW, endH});
                }

                applyOpacity(animation_);

                maybeThrow(NativeNodeApi::getInstance()->insertChildAt(
                    m_nodeHandle, endSharedElementNode.getArkUINodeHandle(), static_cast<int32_t>(-1)));
            } else {
                if (endImageStyle->boundingBox.size.width > startImageStyle->boundingBox.size.width &&
                    endImageStyle->boundingBox.size.height > startImageStyle->boundingBox.size.height) {
                    // endImageStyle 矩形在宽度和高度上都更大
                    facebook::react::Float scaleW = (endImageStyle->boundingBox.size.width - startImageStyle->boundingBox.size.width) *
                                   nodePosition_;
                    facebook::react::Float startW = startImageStyle->boundingBox.size.width + scaleW;

                    facebook::react::Float scaleH = (endImageStyle->boundingBox.size.height - startImageStyle->boundingBox.size.height) *
                                   nodePosition_;
                    facebook::react::Float startH = startImageStyle->boundingBox.size.width + scaleH;

                    float scalePositionX = static_cast<float>(startImageStyle->offset.x) / pixelDensity - scaleW / 2;
                    float scalePositionY = static_cast<float>(startImageStyle->offset.y) / pixelDensity - scaleH / 2;

                    if (startW > endImageStyle->boundingBox.size.width) {
                        startW = endImageStyle->boundingBox.size.width;
                    };
                    if (startH > endImageStyle->boundingBox.size.height) {
                        startH = endImageStyle->boundingBox.size.height;
                    };

                    setPosition({scalePositionX, scalePositionY});
                    setSize({startW, startH});
                    startSharedElementNode.setSize({startW, startH});
                } else if (endImageStyle->boundingBox.size.width < startImageStyle->boundingBox.size.width &&
                           endImageStyle->boundingBox.size.height < startImageStyle->boundingBox.size.height) {
                    // 计算缩放量
                    facebook::react::Float scaleW = (startImageStyle->boundingBox.size.width - endImageStyle->boundingBox.size.width) *
                                   (1 - nodePosition_);
                    facebook::react::Float startW = endImageStyle->boundingBox.size.width + scaleW;

                    facebook::react::Float scaleH = (startImageStyle->boundingBox.size.height - endImageStyle->boundingBox.size.height) *
                                   (1 - nodePosition_);
                    facebook::react::Float startH = endImageStyle->boundingBox.size.height + scaleH;

                    // 计算新的位置，保证中心对齐
                    float scalePositionX = static_cast<float>(endImageStyle->offset.x) / pixelDensity - scaleW / 2;
                    float scalePositionY = static_cast<float>(endImageStyle->offset.y) / pixelDensity - scaleH / 2;

                    // 设定最大值限制
                    if (startW > startImageStyle->boundingBox.size.width) {
                        startW = startImageStyle->boundingBox.size.width;
                    }
                    if (startH > startImageStyle->boundingBox.size.height) {
                        startH = startImageStyle->boundingBox.size.height;
                    }

                    // 设置新的位置和大小
                    setPosition({scalePositionX, scalePositionY});
                    setSize({startW, startH});
                    startSharedElementNode.setSize({startW, startH});
                }

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
        if (animationType == 1 || animationType == 3) {
            setOpacity(0.5);
        } else if (animationType == 2) {
            setOpacity(0.8);
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
