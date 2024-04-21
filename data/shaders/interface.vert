#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

out vec2 UV;

uniform mat4 uProjection;
uniform mat4 uModel = mat4(1);

void main() {
    gl_Position = uProjection * uModel * vec4(aPos, 1.0);
    UV = aUV;
}