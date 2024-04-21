#include "Image.hpp"

#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include "./implementation/stb_image.h"

#include "Logging.hpp"

bool Image::init() {
    stbi_set_flip_vertically_on_load(true);
    return true;
}

unsigned int Image::loadImage(std::string path, glm::ivec2& sizeOut) {
    unsigned int texture = 0;

    int components = 0;

    unsigned char* data = stbi_load(path.c_str(), &sizeOut.x, &sizeOut.y, &components, 4);
    if (data == 0) {
        Logging::error("Failed loading image \"%s\": %s", path.c_str(), stbi_failure_reason);
        return texture;
    }

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sizeOut.x, sizeOut.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}