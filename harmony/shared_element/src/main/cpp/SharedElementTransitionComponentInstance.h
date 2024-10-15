#pragma once

#include "RNOH/CppComponentInstance.h"
#include "RNOH/arkui/StackNode.h"
#include "RNOH/arkui/TextNode.h"
#include "RNTransitionNode.h"
#include "SharedElementTransitionComponentDescriptor.h"
#include "view/RNSharedElementTransition.h"
#include "view/RNSharedElementDrawable.h"

namespace rnoh {
class SharedElementTransitionComponentInstance : public CppComponentInstance<facebook::react::SharedElementTransitionShadowNode>,
                                           public StackNodeDelegate {
public:
    SharedElementTransitionComponentInstance(Context context);

    void onChildInserted(ComponentInstance::Shared const &childComponentInstance, std::size_t index) override;
    void onChildRemoved(ComponentInstance::Shared const &childComponentInstance) override;
    void onPropsChanged(SharedConcreteProps const &props) override;
    ArkUINode &getLocalRootArkUINode() override;
    
    void setViewItem(RNSharedElementTransition::Item item, facebook::react::SharedElementWarpNodeStruct node);
    
    
    void onFinalizeUpdates() override;
    void onLayoutChanged(facebook::react::LayoutMetrics const& layoutMetrics) override;
    
    
    
private:
    RNTransitionNode transitionNode{};
    
    TextNode text{};
    StackNode stack{};
    
    bool validatePosition = false;
    int startHandle = 0;
    int endHandle = 0;
    int direct = 0;
};
} // namespace rnoh