#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Font.hpp"

class Texture {
private:
    unsigned int ID;

    static std::vector<unsigned int> textureList;
    void deletePreviousTexture();

public:
    static void quit();
    static void unbind(Shader*);

    Texture();
    glm::ivec2 setFontString(Font*, std::string);
    glm::ivec2 loadImage(std::string path);
    void bindTexture(Shader*);
    ~Texture();
};