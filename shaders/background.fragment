#version 330 core

out vec4 FragColor;

uniform sampler2D tex;
uniform float scale;
uniform float darknessFactor;

void main()
{
	int	zoomFactor = 32;
    vec4 sampled = vec4(texture(tex, gl_FragCoord.xy / (scale * zoomFactor)).rgb * darknessFactor, 1.0);
    FragColor = sampled;
}
