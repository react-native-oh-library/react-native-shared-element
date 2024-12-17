/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

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
