#version 330 core

in vec2 vUV;

out vec4    fragColor;

uniform mat4  uProj;
uniform mat4  uView;

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

vec3    sampleSkybox(vec3 dir)
{
    vec3    SKY_UP_COLOR = vec3(0.6, 0.8, 1.0);
    vec3    SKY_DOWN_COLOR = vec3(0.4, 0.6, 0.8);

    float   height = clamp(dir.y * 0.5 + 0.5, 0.0, 1.0);

    vec3    sky_color = mix(SKY_UP_COLOR, SKY_DOWN_COLOR, height);

    vec3    SUN_DIR = vec3(0.6, 0.8, 0.4);
    vec3    SUN_COLOR = vec3(1.0);

    vec3    sunDir = normalize(SUN_DIR);
    float   sun_factor = pow(max(dot(dir, sunDir), 0.0), 512.0);

    vec3    sun_color = SUN_COLOR * sun_factor;

    return (clamp(sky_color + sun_color, 0, 1));
}

void main()
{
    vec3 rayOrig;
    vec3 rayDir;
    getRay(rayOrig, rayDir, vUV, uProj, uView);

    vec3    skyColor = sampleSkybox(rayDir);

    fragColor = vec4(skyColor, 1.0);
    gl_FragDepth = 1.0;
}
