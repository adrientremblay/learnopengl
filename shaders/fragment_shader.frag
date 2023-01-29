#version 330 core
out vec4 fragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main() {
   fragColor = mix(texture(texture1, texCoord) * (vec4(ourColor, 1.0) * 1.5f), texture(texture2, texCoord), 0.5);
}