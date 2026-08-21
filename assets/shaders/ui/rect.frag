#version 330 core

out vec4 fragColor;

in vec2 fragPos;

uniform sampler2D uTex;

void main()
{
    vec3 color = texture(uTex, fragPos).rgb;

    color = vec3(1);

    fragColor = vec4(color, 1.0);
}
