#version 440

in vec2 UV;

uniform sampler2D renderedFrame;

out vec4 fragColor;

void main() {
    fragColor = texture(renderedFrame, UV);
}