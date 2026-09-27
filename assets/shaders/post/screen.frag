#version 330 core

in vec2 vUV;

out vec4    fragColor;

uniform sampler2D	uColorFrameBuffer;
uniform sampler2D	uDepthFrameBuffer;

uniform float uNear;
uniform float uFar;
uniform float uTime;

uniform float uScreenWidth;
uniform float uScreenHeight;

uniform bool uDim;
uniform bool uUnderwater;

float random(vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    vec2 u = f * f * (3.0 - 2.0 * f);

    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

vec3	blurImage(int kernelSize, sampler2D tex, vec2 uv)
{
	vec3	color;
	int halfKernel = kernelSize / 2;
	float pixelX = 1.0 / uScreenWidth;
	float pixelY = 1.0 / uScreenHeight;

	for (int x = -halfKernel; x < halfKernel; x++)
	{
		for (int y = -halfKernel; y < halfKernel; y++)
		{
			vec2 offset = vec2(float(x) * pixelX, float(y) * pixelY);
			vec2 ouv = uv + offset;

			ouv.x = clamp(ouv.x, pixelX, 1.0 - pixelX);
			ouv.y = clamp(ouv.y, pixelX, 1.0 - pixelX);

			color += texture(tex, ouv).rgb;
		}
	}

	color /= float(kernelSize * kernelSize);
	return (color);
}

void main()
{
	vec2 zoomedUV = vUV - vec2(0.5);
    zoomedUV = zoomedUV / 1.1;
    zoomedUV = zoomedUV + vec2(0.5);

	float radius = 0.5;
    vec2 circleOffset = vec2(cos(uTime), sin(uTime)) * radius;
    vec2 spinUV = zoomedUV + circleOffset;

	vec2 offset = vec2(noise(spinUV * 2), noise((spinUV + vec2(uTime + 4.2, uTime + 67.69)))) * circleOffset;
	float	intensity = 0.1;
	vec2 warpedUV = zoomedUV + offset * intensity;

	vec2	uv = vUV;

	if (uUnderwater)
		uv = warpedUV;

	vec3 color = texture(uColorFrameBuffer, uv).rgb;

	if (uDim)
	{
		color = blurImage(8, uColorFrameBuffer, uv);
	}

	if (uUnderwater)
		color += vec3(0.0, 0.0, 0.3);

	color = clamp(color, 0, 1);

	fragColor = vec4(color, 1);
}
