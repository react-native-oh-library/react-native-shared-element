/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */
 
#pragma once

#include "RNOH/CppComponentInstance.h"
#include "RNOH/ArkTSTurboModule.h"
#include "RNSharedElementNode.h"
#include "SharedElementTransitionComponentDescriptor.h"

#include <memory>

namespace rnoh {

class RNSharedElementNodeManager {
public:
    RNSharedElementNodeManager(const ArkTSTurboModule::Context ctx): ctx_(ctx){};
    ~RNSharedElementNodeManager(){};

    std::shared_ptr<RNSharedElementNode> acquire(int reactTag, std::shared_ptr<ComponentInstance> view, bool isParent,
                                                 std::shared_ptr<ComponentInstance> ancestor,
                                                 facebook::react::SharedElementNodeStyleCls styleConfig) {
        std::shared_ptr<RNSharedElementNode> node = nullptr;
        auto tmp_node = mNodes.find(reactTag);
        if (tmp_node != mNodes.end()) {
            std::shared_ptr<RNSharedElementNode> node = tmp_node->second;
            node->addRef();
        } else {
            node = std::make_shared<RNSharedElementNode>(reactTag, view, isParent, ancestor, styleConfig);
            mNodes.emplace(reactTag, node);
        }
        return node;
    }

    int release(std::shared_ptr<RNSharedElementNode> node) {
        int refCount = node->releaseRef();
        if (refCount == 0) {
            mNodes.erase(node->getReactTag());
        }
        return refCount;
    }

private:
    ArkTSTurboModule::Context ctx_;
    std::map<int, std::shared_ptr<RNSharedElementNode>> mNodes;
};
}; // namespace rnoh
