//
// Created on 2024/10/12.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".
#include "SharedElementTransitionComponentDescriptor.h"
#include <arkui/native_node.h>

namespace rnoh {
class RNSharedStyle {
public:
    RNSharedStyle(){}
    ~RNSharedStyle(){}
    
    facebook::react::Rect boundingBox;
    ArkUI_AttributeItem sourceItem;
    ArkUI_IntOffset offset;
    
    void setSource(const char* source){
       sourceItem = {.string = source};
    }
    
};
} // namespace rnoh