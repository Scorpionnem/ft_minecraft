#version 330 core

in vec2 vUV;

out vec4    fragColor;

uniform mat4  uProj;
uniform mat4  uView;

uniform vec3 uBoundsMin;
uniform vec3 uBoundsMax;

uniform sampler2D	uFrameBuffer;

uniform float   uTime;

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
	return (0.025);
}

vec4 raymarch(vec3 rayOrig, vec3 rayDir, float dstToBox, float dstInsideBox)
{
	int	MAX_STEPS = 128;
	float	stepSize = 0.3;

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
			color.rgb *= color.a;
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
		fragColor = vec4(background.rgb * (1.0 - cloudColor.a) + cloudColor.rgb, 1.0);
		gl_FragDepth = 1.0;
	}
	else
		discard ;
}
