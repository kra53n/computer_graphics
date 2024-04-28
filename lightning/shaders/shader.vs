#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out vec3 normal;
out vec3 frag_pos;

uniform mat4 m, v, p;

void main()
{
	frag_pos = vec3(m * vec4(a_pos, 1.0));
	normal = mat3(transpose(inverse(m))) * a_normal;
	gl_Position = p * v * m * vec4(a_pos, 1.0);
}