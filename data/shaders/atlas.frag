#version 440 core

in vec2 UV;
uniform sampler2D uAtlasTexture;

out vec4 fragColor;

void main() {
    float transparency = texture(uAtlasTexture, UV).r;
    fragColor = vec4(vec3(1.0), transparency);
}