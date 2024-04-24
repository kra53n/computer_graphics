#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 a_tex_coord;

out vec4 out_col;
out vec2 tex_coord;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(pos, 1.0);
	out_col = vec4(col, 1.);
	tex_coord = a_tex_coord;
}