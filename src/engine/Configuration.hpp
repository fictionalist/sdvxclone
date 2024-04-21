#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace Configuration {
    bool init();
    bool save();
}