/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

// NOTE: This entire file should be codegen'ed.
#pragma once

#include <ReactCommon/TurboModule.h>
#include "RNOH/ArkTSTurboModule.h"
#include "view/RNSharedElementNodeManager.h"

namespace rnoh {

class JSI_EXPORT SharedElementTurbo : public ArkTSTurboModule {
public:
    SharedElementTurbo(const ArkTSTurboModule::Context ctx, const std::string name);

    std::shared_ptr<RNSharedElementNodeManager> getNodeManager() { return nodeManager; }
    
private:
    std::shared_ptr<RNSharedElementNodeManager> nodeManager;
    
};

} // namespace rnoh
