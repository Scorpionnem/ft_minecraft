#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

out vec4    fragColor;

uniform sampler2D uTex;

void main()
{
	float shadowForce = dot(vNormal, vec3(1, 1, 0.5));
	shadowForce = clamp(shadowForce, 0.4, 1.0);

	fragColor = vec4(vColor * shadowForce, 1);
}
