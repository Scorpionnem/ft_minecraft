#version 330 core
out vec4 FragColor;

uniform sampler2D tex;
uniform vec3 color;

in vec2 texPos;

void main()
{
    FragColor = texture(tex, texPos) * vec4(color, 1.0);
}
