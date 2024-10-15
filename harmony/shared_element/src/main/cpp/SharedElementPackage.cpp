#include "SharedElementPackage.h"
#include "SharedElementTransitionComponentInstance.h"
#include "RNOH/RNInstanceCAPI.h"
#include "RNOHCorePackage/ComponentBinders/ViewComponentJSIBinder.h"
#include "RNOHCorePackage/ComponentBinders/ViewComponentNapiBinder.h"
#include "SharedElementTransitionComponentDescriptor.h"
#include "SharedElementTransitionComponentInstance.h"
#include "SharedElementTransitionNapiBinder.h"
#include "SharedElementTransitionJSIBinder.h"
#include "SharedElementTurbo.h"

using namespace rnoh;
using namespace facebook;

std::vector<react::ComponentDescriptorProvider> SharedElementPackage::createComponentDescriptorProviders() {
    return {
        react::concreteComponentDescriptorProvider<react::SharedElementTransitionComponentDescriptor>(),
    };
}

ComponentNapiBinderByString SharedElementPackage::createComponentNapiBinderByName() {
    return {
        {"RNSharedElementTransition", std::make_shared<SharedElementTransitionNapiBinder>()},
    };
};

ComponentJSIBinderByString SharedElementPackage::createComponentJSIBinderByName() {
    return {
        {"RNSharedElementTransition", std::make_shared<SharedElementTransitionJSIBinder>()},
    };
}


ComponentInstance::Shared SharedElementPackage::createComponentInstance(const ComponentInstance::Context &ctx) {
    if (ctx.componentName == "RNSharedElementTransition") {
        return std::make_shared<SharedElementTransitionComponentInstance>(ctx);
    }
    return nullptr;
};

class SharedElementTurboModuleFactoryDelegate : public TurboModuleFactoryDelegate {
public:
    SharedTurboModule createTurboModule(Context ctx, const std::string &name) const override {
        if (name == "RNSharedElementTransition") {
            return std::make_shared<SharedElementTurbo>(ctx, name);
        }
        return nullptr;
    };
};

std::unique_ptr<TurboModuleFactoryDelegate> SharedElementPackage::createTurboModuleFactoryDelegate() {
    return std::make_unique<SharedElementTurboModuleFactoryDelegate>();
};