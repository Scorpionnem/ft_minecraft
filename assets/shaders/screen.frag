#version 330 core

in vec2 vUV;

out vec4    fragColor;

uniform sampler2D	uFrameBuffer;

void main()
{
	fragColor = vec4(texture(uFrameBuffer, vUV).rgb, 1.0);
}
