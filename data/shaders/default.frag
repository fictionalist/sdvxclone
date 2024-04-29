#version 440 core

in vec2 UV;

uniform int uHasTexture = 0;
uniform sampler2D uTexture;

uniform int uTime = 0;
uniform int uModelID = 0;
uniform vec4 uColor = vec4(1.0);

out vec4 fragColor;

void main() {
    int phase = uTime + (370 * uModelID);
    vec4 rainbow = vec4(0.5 + (0.5 * cos(radians(phase / 10.0))), 0.5 + (0.5 * cos(radians((phase / 10.0) + 120.0))), 0.5 + (0.5 * cos(radians((phase / 10.0) + 240.0))), 1.0);
    if (uHasTexture == 1) {
        fragColor = texture(uTexture, UV) * uColor;
    } else {
        fragColor = uColor;
    }
}
