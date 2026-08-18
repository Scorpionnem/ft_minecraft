#version 330 core

out vec4 fragColor;

in vec2    vUV;

uniform float time;

void    main()
{
    vec3    color = vec3(vUV.x, vUV.y, 0.0);

    fragColor = vec4(color, 1.0);
}
