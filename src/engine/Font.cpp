#include "Font.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Logging.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

static FT_Library library;
static Shader* fontShader = nullptr;

bool Font::init() {
    FT_Error error = FT_Init_FreeType(&library);
    if (error) {
        Logging::error("Failed to initialize FreeType: %s", FT_Error_String(error));
        return false;
    }
    return true;
}

void Font::quit() {
    FT_Done_FreeType(library);
}

void Font::setShader(Shader* s) {
    fontShader = s;
}

void Font::generateAtlasTexture() {
    FT_GlyphSlot slot = face->glyph;
    fontAtlasDimensions = glm::ivec2(0);

    for (unsigned int i = 0; i < 256; i++) {
        FT_Error loadErr = FT_Load_Char(face, i, FT_LOAD_RENDER);
        if (loadErr != FT_Err_Ok) {
            continue;
        }

        fontAtlasDimensions.x += slot->bitmap.width;
        if (fontAtlasDimensions.y < slot->bitmap.rows) {
            fontAtlasDimensions.y = slot->bitmap.rows;
        }
    }

    //printf("Font atlas dimensions: %d x %d\n", fontAtlasDimensions.x, fontAtlasDimensions.y);

    glGenTextures(1, &fontAtlasTexture);
    glBindTexture(GL_TEXTURE_2D, fontAtlasTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fontAtlasDimensions.x, fontAtlasDimensions.y, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    glm::ivec2 pen = glm::ivec2(0);

    FT_Matrix flipMatrix = {
        0x10000, 0,
        0, -0x10000
    };
    FT_Set_Transform(face, &flipMatrix, nullptr);

    for (unsigned int i = 0; i < 256; i++) {
        FT_Error loadErr = FT_Load_Char(face, i, FT_LOAD_RENDER);
        if (loadErr != FT_Err_Ok) {
            continue;
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0, pen.x, 0, slot->bitmap.width, slot->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);

        Character ch = {};
        ch.textureSize = glm::vec2((float)slot->bitmap.width / (float)fontAtlasDimensions.x, (float)slot->bitmap.rows / (float)fontAtlasDimensions.y);
        ch.textureOffset = glm::vec2((float)pen.x / (float)fontAtlasDimensions.x, (float)pen.y / (float)fontAtlasDimensions.y);
        ch.size = glm::ivec2(slot->bitmap.width, slot->bitmap.rows);
        ch.advance = glm::ivec2(slot->advance.x, slot->advance.y);
        ch.bearing = glm::ivec2(slot->bitmap_left, slot->bitmap_top);

        characterMap.insert(std::make_pair(i, ch));

        pen.x += slot->bitmap.width;
    }

    FT_Matrix identityMatrix = {
        0x10000, 0,
        0, 0x10000
    };
    FT_Set_Transform(face, &identityMatrix, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

unsigned int Font::getBaseline(std::string str) {
    unsigned int baseline = 0;

    bool found = false;
    Character ch;
    for (auto& c : str) {
        ch = characterMap[c];
        unsigned int bl = ch.size.y - ch.bearing.y;
        if (baseline < bl) {
            baseline = bl;
        }
    }
    return baseline;
}

glm::ivec2 Font::getStringBoundingBox(std::string str) {
	int bufferWidth = 0;
	int	bufferHeight = 0;
	int bearingYCorrection = 0;

	bool found = false;
	Character ch = {};

	for (auto& c : str) {
		ch = characterMap[c];
		bufferWidth += (ch.advance.x >> 6);
		bearingYCorrection = (2 * ch.size.y) - ch.bearing.y;
		if (bufferHeight < bearingYCorrection) {
			bufferHeight = bearingYCorrection;
		}
	}
	return glm::ivec2(bufferWidth, bufferHeight);
}

unsigned int Font::generateStringTexture(std::string str) {
    glm::ivec2 stringDimensions = getStringBoundingBox(str);

    unsigned int stringTexture;
    glGenTextures(1, &stringTexture);

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    glBindTexture(GL_TEXTURE_2D, stringTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, stringDimensions.x, stringDimensions.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, stringTexture, 0);

    unsigned int renderbuffer;
    glGenRenderbuffers(1, &renderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, stringDimensions.x, stringDimensions.y);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);

    glViewport(0, 0, stringDimensions.x, stringDimensions.y);

    unsigned int err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (err != GL_FRAMEBUFFER_COMPLETE) {
        Logging::error("Framebuffer incomplete: %d", err);
        glDeleteTextures(1, &stringTexture);
        glDeleteFramebuffers(1, &framebuffer);
        Renderer::resetFramebuffer();
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	unsigned int VAO;
	unsigned int VBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), 0, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, (4 * sizeof(float)), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (4 * sizeof(float)), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glm::mat4 projection = glm::ortho(0.0f, (float)stringDimensions.x, (float)stringDimensions.y, 0.0f, -1.0f, 1.0f);

	Renderer::atlasShader->use();
	Renderer::atlasShader->setMat4("uProjection", projection);

	glBindTexture(GL_TEXTURE_2D, fontAtlasTexture);
    glActiveTexture(GL_TEXTURE0);
    Renderer::atlasShader->setInt("uAtlasTexture", 0);

	Character* character;
	float penX = 0.0f;
	float penY = (float)getBaseline(str);
	float xpos = 0.0f;
	float ypos = 0.0f;

	for (auto it = str.begin(); it != str.end(); it++) {
		character = &(characterMap[*it]);
		xpos = penX;
		ypos = penY - (float)character->bearing.y;

        /*
		float vertices[6][4] = {
			{xpos, ypos, character->textureOffset.x, character->textureOffset.y},
			{xpos + (float)character->size.x, ypos, character->textureOffset.x + character->textureSize.x, character->textureOffset.y},
			{xpos, ypos + (float)character->size.y, character->textureOffset.x, character->textureOffset.y + character->textureSize.y},
			{xpos + (float)character->size.x, ypos, character->textureOffset.x + character->textureSize.x, character->textureOffset.y},
			{xpos + (float)character->size.x, ypos + (float)character->size.y, character->textureOffset.x + character->textureSize.x, character->textureOffset.y + character->textureSize.y},
			{xpos, ypos + (float)character->size.y, character->textureOffset.x, character->textureOffset.y + character->textureSize.y}
		};
        */

       float vertices[6][4] = {
            {xpos,                      ypos, character->textureOffset.x,                            character->textureOffset.y},
            {xpos + character->size.x,  ypos, character->textureOffset.x + character->textureSize.x, character->textureOffset.y},
            {xpos + character->size.x,  ypos + character->size.y, character->textureOffset.x + character->textureSize.x, character->textureOffset.y + character->textureSize.y},
            {xpos,                      ypos, character->textureOffset.x,                            character->textureOffset.y},
            {xpos + character->size.x,  ypos + character->size.y, character->textureOffset.x + character->textureSize.x, character->textureOffset.y + character->textureSize.y},
            {xpos,                      ypos + character->size.y, character->textureOffset.x,                            character->textureOffset.y + character->textureSize.y},
       };

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		penX += (float)(character->advance.x >> 6);
	}

    glDeleteFramebuffers(1, &framebuffer);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Renderer::resetFramebuffer();

    return stringTexture;
}

Font::Font(std::string path, unsigned int size) {
    FT_Error err = FT_New_Face(library, path.c_str(), 0, &face);
    if (err != FT_Err_Ok) {
        Logging::error("Failed to load font \"%s\": %s", path.c_str(), FT_Error_String(err));
        return;
    }

    err = FT_Set_Pixel_Sizes(face, 0, size);

    if (err != FT_Err_Ok) {
        Logging::error("Failed to load font \"%s\": %s", path.c_str(), FT_Error_String(err));
        FT_Done_Face(face);
        return;
    }

    bool hasKerning = FT_HAS_KERNING(face);
    if (hasKerning) printf("has kerning\n");
    generateAtlasTexture();
}

Font::~Font() {
    glDeleteTextures(stringTextures.size(), &stringTextures[0]);
    stringTextures.clear();
    glDeleteTextures(1, &fontAtlasTexture);
    characterMap.clear();
}