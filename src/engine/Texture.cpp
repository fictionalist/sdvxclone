#include "Texture.hpp"

#include <string>
#include <vector>
#include <algorithm>

#include <GL/glew.h>

#include "Image.hpp"
#include "Font.hpp"

std::vector<unsigned int> Texture::textureList = std::vector<unsigned int>();

Texture::Texture() {
    ID = 0;
}

Texture::~Texture() {
    std::remove(textureList.begin(), textureList.end(), ID);
    glDeleteTextures(1, &ID);
}

void Texture::deletePreviousTexture() {
    if (ID != 0) {
        std::remove(textureList.begin(), textureList.end(), ID);
        glDeleteTextures(1, &ID);
    }
}

glm::ivec2 Texture::setFontString(Font* f, std::string str) {
    deletePreviousTexture();
    ID = f->generateStringTexture(str);
    textureList.push_back(ID);
    return f->getStringBoundingBox(str);
}

glm::ivec2 Texture::loadImage(std::string path) {
    deletePreviousTexture();
    glm::ivec2 size;
    ID = Image::loadImage(path, size);
    textureList.push_back(ID);
    return size;
}

void Texture::bindTexture(Shader* sh) {
    sh->setInt("uHasTexture", 1);
    glBindTexture(GL_TEXTURE_2D, ID);
    glActiveTexture(GL_TEXTURE0);
    sh->setInt("uTexture", 0);
}

void Texture::unbind(Shader* sh) {
    sh->setInt("uHasTexture", 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::quit() {
    glDeleteTextures(textureList.size(), &textureList[0]);
    textureList.clear();
}