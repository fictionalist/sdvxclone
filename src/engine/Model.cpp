#include "Model.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.hpp"
#include "Texture.hpp"

Model::Model() {
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    setShader(Renderer::baseShader);
    resetTransform();
}

void Model::resetTransform() {
    glm::mat4 matrix = glm::mat4(1.0f);
    matrix = glm::translate(matrix, position);
    matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transformMatrix = matrix;
}

void Model::setPosition(glm::vec3 p) {
    position = p;
    resetTransform();
}

void Model::setRotation(glm::vec3 r) {
    rotation = r;
    resetTransform();
}

void Model::draw() {
    if (VBO == 0) {
        return;
    }

    shader->use();
    shader->setInt("uModelID", ID);
    shader->setMat4("uModel", transformMatrix);
    
    if (texture != nullptr) {
        texture->bindTexture(shader);
    } else {
        Texture::unbind(shader);
    }
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
}