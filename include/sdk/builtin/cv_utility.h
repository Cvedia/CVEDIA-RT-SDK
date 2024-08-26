#pragma once

#include <string>
#include "defines.h"

namespace cvedia {
    namespace rt {
        namespace builtin {
            EXPORT int stringToInterpolationMethod(std::string method, std::string const& defaultMethod = "nearest");
        }
    }
}