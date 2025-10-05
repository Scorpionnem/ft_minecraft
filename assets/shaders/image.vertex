#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;

uniform mat4 model;
uniform mat4 projection;

out vec2 fragUV;

void main()
{
    fragUV = vec2(inUV.x, -inUV.y);
    gl_Position = projection * model * vec4(inPos, 0.0, 1.0);
}
