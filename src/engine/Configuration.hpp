#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace Configuration {
    extern nlohmann::json settings;
    
    bool init();
    bool save();
}