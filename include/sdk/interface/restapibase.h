#pragma once

#include <memory>
#include <string>
#include "defines.h"

namespace cvedia::rt
{
    class RestApiBase {
    public:
        EXPORT explicit RestApiBase() {};
        EXPORT virtual ~RestApiBase() = default;
        EXPORT virtual void init() = 0;
        EXPORT virtual std::string getYaml() const = 0;
        EXPORT void setYamlOverride(std::string const& yaml) {
            overrideYaml_ = yaml;
            useOverride_ = true;
        }
        std::string overrideYaml_;
        bool useOverride_ = false;
    };
}
