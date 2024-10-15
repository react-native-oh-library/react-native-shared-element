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
