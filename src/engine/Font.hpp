#pragma once

#include <string>

class Font {
private:
    void generateAtlasTexture();
public:
    static bool init();
    Font(std::string path);
};