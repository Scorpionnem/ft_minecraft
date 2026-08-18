#version 330 core

in vec2 vUV;

out vec4    fragColor;

uniform mat4  uProj;
uniform mat4  uView;

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

vec3    sampleSkybox(vec3 dir)
{
    vec3    SKY_DOWN_COLOR = vec3(0.6, 0.8, 1.0);
    vec3    SKY_UP_COLOR = vec3(0.4, 0.6, 0.8);

    vec3    SUN_COLOR = vec3(1.0);

    float   height = clamp(dir.y * 0.5 + 0.5, 0.0, 1.0);

    vec3    sky_color = mix(SKY_UP_COLOR, SKY_DOWN_COLOR, height);

    vec3    sunDir = normalize(vec3(0.6, 0.8, 0.4));
    float   sun_factor = pow(max(dot(dir, sunDir), 0.0), 512.0);

    return (clamp(sky_color + SUN_COLOR * sun_factor, 0, 1));
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

void main()
{
    vec3 rayOrig;
    vec3 rayDir;
    getRay(rayOrig, rayDir, vUV, uProj, uView);

    vec3    skyColor = sampleSkybox(rayDir);

    float   dstToBox;
    float   dstInsideBox;
    rayBoxDst(dstToBox, dstInsideBox, vec3(0), vec3(10), rayOrig, rayDir);

    bool rayHitBox = dstInsideBox > 0;
    if (rayHitBox)
    {
        float   densityMultiplier = 1;
        float   density = dstInsideBox * densityMultiplier;
        float   transmittance = exp(-density);
        fragColor = vec4(skyColor * transmittance, 1.0);
        return ;
    }

    fragColor = vec4(skyColor, 1.0);
}
