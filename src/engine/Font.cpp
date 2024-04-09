#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>

#include "Logging.hpp"

static FT_Library library;

bool Font::init() {
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        Logging::error("Failed to initialize FreeType: %s", FT_Error_String(error));
        return false;
    }
    return true;
}

void Font::generateAtlasTexture() {
    
}

Font::Font(std::string path) {

}