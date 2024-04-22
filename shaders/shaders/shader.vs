#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 col;

out vec4 out_col;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	out_col = vec4(col, 1.);
}