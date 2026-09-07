#version 330 core

out vec4 fragColor;

in vec2 fragPos;

uniform sampler2D uTex;

uniform vec3	uColor;
uniform vec4	uUV;
uniform bool	uBackground;
uniform vec3	uBackgroundColor;

void main()
{
    vec2 uv = mix(uUV.xy, uUV.zw, fragPos);
    float a = texture(uTex, uv).a;
    if (a <= 0)
    {
    	if (uBackground)
     		fragColor = vec4(uBackgroundColor, 0.5);
     	else
      		discard ;
    }
    else
    	fragColor = vec4(uColor, a);
}
