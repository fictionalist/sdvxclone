#pragma once

#include <string>

#include <glm/glm.hpp>

namespace Image {
    bool init();
    unsigned int loadImage(std::string path, glm::ivec2& sizeOut);
}