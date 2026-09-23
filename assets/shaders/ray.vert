#version 330 core

uniform mat4 uView;
uniform mat4 uProj;

uniform vec3 uPosA;
uniform vec3 uPosB;

void main()
{
	vec3 pos = (gl_VertexID == 0) ? uPosA : uPosB;
    gl_Position = uProj * uView * vec4(pos, 1.0);
}
