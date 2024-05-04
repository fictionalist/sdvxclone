#include "Model.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.hpp"
#include "Texture.hpp"

Model::Model() {
    init();
}

void Model::draw(Shader* shader, Texture* texture) {
    if (VBO == 0) return;
    
    if (texture != nullptr) {
        texture->bindTexture(shader);
    } else {
        Texture::unbind(shader);
    }
    shader->setInt("uModel", ID);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
}