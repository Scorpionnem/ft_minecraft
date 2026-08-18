#version 330 core

in vec2 vUV;

out vec4    fragColor;

uniform mat4  uProj;
uniform mat4  uView;

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

    float   dstToBox;
    float   dstInsideBox;
    rayBoxDst(dstToBox, dstInsideBox, vec3(0), vec3(10), rayOrig, rayDir);

    vec3    color = vec3(1); // post process so sample from texture that was there

    bool rayHitBox = dstInsideBox > 0;
    if (rayHitBox)
    {
        float   densityMultiplier = 1;
        float   density = dstInsideBox * densityMultiplier;
        float   transmittance = exp(-density);
        fragColor = vec4(color * transmittance, 1.0);
    }
    else
        discard ;
}
