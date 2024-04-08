#pragma once

#include <cstdarg>
#include <string>

namespace Logging {
    bool init();
    void error(std::string fmt, ...);
}