#version 330 core

uniform vec3 uCol;

out vec4 fragColor;

void main()
{
	fragColor = vec4(uCol, 1.0);
}
