#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 texCoord;

uniform vec3 aPosOffset;

void main() {
   gl_Position = vec4(aPos.x + aPosOffset.x, aPos.y + aPosOffset.y, aPos.z + aPosOffset.z, 1.0);
   ourColor = aColor;
   texCoord = aTexCoord;
}
