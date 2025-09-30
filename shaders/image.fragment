#version 330 core

in vec2 fragUV;
out vec4 FragColor;

uniform sampler2D tex;

void main()
{
    vec4 sampled = texture(tex, fragUV);
    FragColor = sampled;
}
