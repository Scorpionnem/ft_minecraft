#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;

out vec4    fragColor;

uniform sampler2D uTex;

void main()
{
	fragColor = vec4(abs(vNormal), 1);
}
