#include "RNOH/Package.h"

namespace rnoh {

class SharedElementPackage : public Package {
 public:
  SharedElementPackage(Package::Context ctx) : Package(ctx) {}

  std::vector<facebook::react::ComponentDescriptorProvider> createComponentDescriptorProviders() override;

  ComponentNapiBinderByString createComponentNapiBinderByName() override;

  ComponentJSIBinderByString createComponentJSIBinderByName() override;
    
  ComponentInstance::Shared createComponentInstance(const ComponentInstance::Context& ctx) override;
    
  std::unique_ptr<TurboModuleFactoryDelegate> createTurboModuleFactoryDelegate() override;
};
} // namespace rnoh
