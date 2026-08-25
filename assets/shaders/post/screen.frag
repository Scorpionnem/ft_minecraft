#version 330 core

in vec2 vUV;

out vec4    fragColor;

uniform sampler2D	uColorFrameBuffer;
uniform sampler2D	uDepthFrameBuffer;

uniform int	uScreenWidth;
uniform int	uScreenHeight;

uniform float uNear;
uniform float uFar;

float	linearizeDepth(float depth, float zNear, float zFar)
{
    float zNDC = depth * 2.0 - 1.0;
    return (2.0 * zNear * zFar) / (zFar + zNear - zNDC * (zFar - zNear));
}

float	makeDepthNormal(float depth, float zNear, float zFar)
{
	return (1.0 - (linearizeDepth(depth, zNear, zFar) - zNear) / (zFar - zNear));
}

void main()
{
	float	depth = makeDepthNormal(texture(uDepthFrameBuffer, vUV).r, uNear, uFar);
	vec3	color = texture(uColorFrameBuffer, vUV).rgb;

	fragColor = vec4(color, 1.0);
}
