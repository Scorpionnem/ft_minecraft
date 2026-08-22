#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;

out vec4    fragColor;

uniform sampler2D uTex;

void main()
{
	vec4	color = texture(uTex, vUv);
	if (color.a <= 0)
		discard ;
	fragColor = color;
}
