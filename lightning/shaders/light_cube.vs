#version 330 core
layout (location = 0) in vec3 pos;

uniform mat4 pv, m;

void main()
{
	gl_Position = pv * m * vec4(pos, 1.0);
}