#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 UV;

uniform mat4 uProjection;
uniform mat4 uModel = mat4(1.0);
uniform mat4 uView = mat4(1.0);

void main() {
    gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0);
    UV = aTexCoord;
}