#version 330 core

in vec2 vUV;

out vec4    fragColor;

uniform sampler2D	uColorFrameBuffer;
uniform sampler2D	uDepthFrameBuffer;

uniform int	uScreenWidth;
uniform int	uScreenHeight;

float	linearizeDepth(float depth, float zNear, float zFar)
{
    float zNDC = depth * 2.0 - 1.0;
    return (2.0 * zNear * zFar) / (zFar + zNear - zNDC * (zFar - zNear));
}

float	makeDepthNormal(float depth, float zNear, float zFar)
{
	return (1.0 - (linearizeDepth(depth, zNear, zFar) - zNear) / (zFar - zNear));
}

bool	outline()
{
	float pixelX = 1.0 / 860;
	float pixelY = 1.0 / 520;

	int kernelSize = 4;
	int halfKernel = kernelSize / 2;

	float	avg = 0;

	int	count = 0;

	for (int x = -halfKernel; x < halfKernel; x++)
	{
		for (int y = -halfKernel; y < halfKernel; y++)
		{
			if (x == 0 && y == 0)
				continue ;

			vec2 offset = vec2(float(x) * pixelX, float(y) * pixelY);
			count++;
			avg += makeDepthNormal(texture(uDepthFrameBuffer, vUV + offset).r, 0.01, 1000.0);
		}
	}

	avg /= count;

	float	depth = makeDepthNormal(texture(uDepthFrameBuffer, vUV).r, 0.01, 1000.0);

	float	diff = depth - avg;

	float	e = 0.01;

	return (diff > e);
}

void main()
{
	float zNear = 0.01;
	float zFar = 1000.0;
	float	depth = makeDepthNormal(texture(uDepthFrameBuffer, vUV).r, zNear, zFar);
	vec3	color = texture(uColorFrameBuffer, vUV).rgb;
	if (outline())
		color = vec3(0);

	fragColor = vec4(color, 1.0);
}
