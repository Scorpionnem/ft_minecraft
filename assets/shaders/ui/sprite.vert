#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 uProj;
uniform mat4 uModel;

out vec2 fragPos;
out vec2 vUV;

void main() {
    fragPos = vec2(aPos.x, 1.0 - aPos.y);
    vUV = vec2(aUV.x, 1.0 - aUV.y);

    gl_Position = uProj * uModel * vec4(aPos, 0.0, 1.0);
}
