#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;

out vec4    fragColor;

void main()
{
	fragColor = vec4(mod(vFragPos, 1.0), 1);
}
