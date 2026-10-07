#version 330 core

out vec4 fragColor;

in vec2 fragPos;
in vec2 vUV;

uniform sampler2D uTex;
uniform bool uTile;

void main()
{
	vec4 color = vec4(1);
	if (uTile)
	{
		color = texture(uTex, gl_FragCoord.xy / 100);
	}
	else
		color = texture(uTex, vUV.xy);
    fragColor = color;
}
