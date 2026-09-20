#version 330 core

out vec4 fragColor;

in vec2 fragPos;

uniform sampler2D uTex;

uniform vec3	uColor;
uniform bool	uUseTex;
uniform vec2	uRectSize;

uniform vec2 uTexSize;
uniform vec2 uBorder;

float slice(float q, float quadSize, float texSize, float border)
{
    if (q < border)
        return (q);
    if (q > quadSize - border)
        return (texSize - (quadSize - q));
    float	inner = texSize - 2.0 * border;
    return (border + mod(q - border, inner));
}

void main()
{
    vec2 q = fragPos * uRectSize;
    vec2 tp = vec2(
        slice(q.x, uRectSize.x, uTexSize.x, uBorder.x),
        slice(q.y, uRectSize.y, uTexSize.y, uBorder.y)
    );
    vec2 uv = tp / uTexSize;
    vec4 color = texture(uTex, uv);
    fragColor = color;
}
