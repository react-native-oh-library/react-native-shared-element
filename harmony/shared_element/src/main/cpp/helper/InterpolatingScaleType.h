//
// Created on 2024/9/25.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".
#include "view/RNSharedElementTypes.h"
#include "Rect.h"

namespace rnoh {
class InterpolatingScaleType {
public:
    InterpolatingScaleType(ScaleType scaleFrom, ScaleType scaleTo, std::shared_ptr<drawing::Rect> boundsFrom, std::shared_ptr<drawing::Rect> boundsTo){
        // todo
    };
    
    void setValue(float position){
        // todo
    }
    
    ScaleType getScaleType(){
        // todo
        return ScaleType::MATRIX;
    }
};
}; // namespace rnoh
