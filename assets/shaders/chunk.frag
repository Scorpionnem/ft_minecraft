#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;

out vec4    fragColor;

uniform sampler2D uTex;

void main()
{
	vec4	color = texture(uTex, vUv);
	fragColor = vec4(abs(vNormal), 1);
}
