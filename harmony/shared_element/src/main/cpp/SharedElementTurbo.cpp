#include "SharedElementTurbo.h"
#include "SharedElementManager.h"

using namespace rnoh;
using namespace facebook;

static jsi::Value __hostFunction_SharedElementTurbo_configure(jsi::Runtime &rt, react::TurboModule &turboModule,
                                                              const jsi::Value *args, size_t count) {
    SharedElementManager::getInstance().runtime = &rt;
    return jsi::Value(true);
}

SharedElementTurbo::SharedElementTurbo(const ArkTSTurboModule::Context ctx, const std::string name)
    : ArkTSTurboModule(ctx, name) {
    nodeManager = std::make_shared<RNSharedElementNodeManager>(ctx);
    methodMap_["configure"] = MethodMetadata{1, __hostFunction_SharedElementTurbo_configure};
}