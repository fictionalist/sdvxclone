#pragma once

#include <cstdarg>
#include <string>

namespace Logging {
    bool init();
    void info(std::string fmt, ...);
    void error(std::string fmt, ...);
}