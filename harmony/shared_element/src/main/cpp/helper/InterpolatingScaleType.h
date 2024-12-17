/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

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
