#pragma once

#include <string>
#include <vector>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>

#include "Shader.hpp"

struct Character {
    glm::ivec2 size;
    glm::ivec2 bearing;
    glm::ivec2 advance;
    glm::vec2 textureOffset;
    glm::vec2 textureSize;
};

class Font {
private:
    FT_Face face;

    unsigned int fontVBO;
    unsigned int fontVAO;
    unsigned int fontAtlasTexture;
    glm::ivec2 fontAtlasDimensions;

    std::map<wchar_t, Character> characterMap;
    std::vector<unsigned int> stringTextures;

    void generateAtlasTexture();

    unsigned int getBaseline(std::string);

public:
    static bool init();
    static void setShader(Shader*);
    static void quit();
    Font(std::string path, unsigned int size);
    ~Font();
    glm::ivec2 getStringBoundingBox(std::string);
    unsigned int generateStringTexture(std::string);
};