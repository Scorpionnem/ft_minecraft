#version 330 core

in vec2 vUV;

out vec4    fragColor;

uniform mat4  uProj;
uniform mat4  uView;

uniform vec3 uBoundsMin;
uniform vec3 uBoundsMax;

uniform float uNear;
uniform float uFar;

uniform sampler2D	uFrameBuffer;

uniform float   uTime;

#include "../common/noise.glsl"

void    getRay(out vec3 rayOrig, out vec3 rayDir, vec2 uv, mat4 proj, mat4 view)
{
    vec2 clipSpace = vUV * 2.0 - 1.0;

    mat4    invViewProj = inverse(proj * view);
    vec4 nearPointWorld = invViewProj * vec4(clipSpace, -1.0, 1.0);
    vec4 farPointWorld  = invViewProj * vec4(clipSpace,  1.0, 1.0);
    nearPointWorld /= nearPointWorld.w;
    farPointWorld /= farPointWorld.w;

    rayOrig = inverse(uView)[3].xyz;
    rayDir = normalize(farPointWorld.xyz - nearPointWorld.xyz);
}

float	getOpenGLDepth(float z, float near, float far)
{
	return ((1 / z - 1 / near) / (1 / far - 1 / near));
}

// rayHitBox == dstInsideBox > 0
void    rayBoxDst(out float dstToBox, out float dstInsideBox, vec3 boundsMin, vec3 boundsMax, vec3 rayOrig, vec3 rayDir)
{
    vec3   t0 = (boundsMin - rayOrig) / rayDir;
    vec3   t1 = (boundsMax - rayOrig) / rayDir;
    vec3   tmin = min(t0, t1);
    vec3   tmax = max(t0, t1);

    float   dstA = max(max(tmin.x, tmin.y), tmin.z);
    float   dstB = min(min(tmax.x, tmax.y), tmax.z);

    dstToBox = max(0, dstA);
    dstInsideBox = max(0, dstB - dstToBox);
}

float	cloudDensity(vec3 pos)
{
	float	n = frac_noise(pos, 0.05, 0.35, 3);

	float	halfHeight = (uBoundsMax.y - uBoundsMin.y) / 2.0;
	float	middle = (uBoundsMax.y + uBoundsMin.y) / 2.0;
	float	dist = abs(middle - pos.y);
	float	dist_weight = (1.0 - clamp(dist / halfHeight, 0.0, 1.0));

	vec2	v_halfDist = (uBoundsMax.xz - uBoundsMin.xz) / 2.0;
	vec2	v_middle = (uBoundsMax.xz + uBoundsMin.xz) / 2.0;
	vec2	v_dist = abs(v_middle - pos.xz);
	vec2	v_dist_weight = (vec2(1.0) - clamp(v_dist / v_halfDist, 0.0, 1.0));

	float edge = 0.1;

	float v_w = clamp(dist_weight / edge, 0.0, 1.0);
	vec2  h_w = clamp(v_dist_weight / edge, 0.0, 1.0);

	float total_weight = v_w * h_w.x * h_w.y;

	n *= total_weight;

	float	e = 0.3;

	if (n < e)
		return (0);
	return (n * 0.05);
}

vec4 raymarch(vec3 rayOrig, vec3 rayDir, float dstToBox, float dstInsideBox)
{
	int	MAX_STEPS = 128;
	float	stepSize = dstInsideBox / MAX_STEPS;
	stepSize = clamp(stepSize, 0.01, 10.0);

	float advance = dstToBox;
	float end = dstToBox + dstInsideBox;

	float	transmittance = 1.0;
	vec4	res = vec4(0);

	for (int i = 0; i < MAX_STEPS; i++)
	{
		if (advance > end)
			break ;

		vec3 pos = rayOrig + advance * rayDir;
		float density = cloudDensity(pos);

		if (density > 0.0)
		{
			vec4 color = vec4(1.0, 1.0, 1.0, density);
			res += color * (1.0 - res.a);
		}

		advance += stepSize;
	}

	return (res);
}

void main()
{
    vec3 rayOrig;
    vec3 rayDir;
    getRay(rayOrig, rayDir, vUV, uProj, uView);

	float	dstToBox;
	float	dstInsideBox;
	rayBoxDst(dstToBox, dstInsideBox, uBoundsMin, uBoundsMax, rayOrig, rayDir);

	bool	rayHitBox = dstInsideBox > 0;
	if (rayHitBox)
	{
		vec4	background = texture(uFrameBuffer, vUV);

		vec4	cloudColor = raymarch(rayOrig, rayDir, dstToBox, dstInsideBox);
		fragColor = vec4(cloudColor.rgb, cloudColor.a);
		gl_FragDepth = getOpenGLDepth(dstToBox, uNear, uFar);
	}
	else
		discard ;
}
