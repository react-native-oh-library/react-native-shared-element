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
