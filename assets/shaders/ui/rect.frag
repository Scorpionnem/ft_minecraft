#version 330 core

out vec4 fragColor;

in vec2 fragPos;

uniform sampler2D uTex;

uniform vec3	uColor;
uniform vec4	uUV;
uniform bool	uUseTex;

void main()
{
    if (uUseTex)
    {
        vec2 uv = mix(uUV.xy, uUV.zw, fragPos);
        float a = texture(uTex, uv).a;
        fragColor = vec4(uColor, a);
    }
    else
    {
        fragColor = vec4(uColor, 1.0);
    }
}
