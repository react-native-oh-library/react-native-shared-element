/*
 * Copyright (c) 2024 Huawei Device Co., Ltd. All rights reserved
 * Use of this source code is governed by a MIT license that can be
 * found in the LICENSE file.
 */

#pragma once

#include "RNOH/CppComponentInstance.h"


#include <memory>
#include <rawfile/raw_file_manager.h>
#include <sys/stat.h>

namespace rnoh {


class SharedElementManager {
public:
    static SharedElementManager &getInstance() {
        static SharedElementManager instance;
        return instance;
    }
    facebook::jsi::Runtime *runtime;
    

private:
    SharedElementManager() {}
    SharedElementManager(const SharedElementManager &) = delete;
    SharedElementManager &operator=(const SharedElementManager &) = delete;
};

} // namespace rnoh
