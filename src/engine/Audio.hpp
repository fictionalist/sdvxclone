#pragma once

#include <string>

namespace Audio {
    bool init();
    void quit();

    unsigned int loadFile(std::string path);
}