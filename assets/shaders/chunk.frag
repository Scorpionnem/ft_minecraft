#version 330 core

in vec3 vFragPos;
in vec3 vNormal;
in vec2 vUv;
in vec3 vColor;

out vec4    fragColor;

uniform sampler2D uAtlas;

void main()
{
	float shadowForce = dot(vNormal, vec3(1, 1, 0.5));
	shadowForce = clamp(shadowForce, 0.4, 1.0);

	vec4	color = texture(uAtlas, vUv);

	if (color.a == 0)
		discard ;

	fragColor = vec4(color.rgb * vColor * shadowForce, 1);
}
