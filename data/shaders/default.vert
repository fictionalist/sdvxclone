#version 400 core
layout (location = 0) in vec3 aPos;

uniform mat4 uProjection = mat4(1.0);

void main() {
    gl_Position = uProjection * vec4(aPos, 1.0);
}