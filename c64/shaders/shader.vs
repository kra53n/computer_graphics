#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;
layout (location = 2) in vec2 a_tex_coords;

out vec3 normal;
out vec3 frag_pos;
out vec3 view_light_pos;
out vec2 tex_coords;

uniform vec3 light_pos;
uniform mat4 m, v, vm, pv;
uniform mat2 rotation_matrix_for_screen_texture;

uniform int verts_type;
const int VERTEX_TYPE_OTHER = 0;
const int VERTEX_TYPE_SCREEN = 1;

void main()
{
	gl_Position = pv * m * vec4(a_pos, 1.0);

	frag_pos = vec3(m * vec4(a_pos, 1.0));
	normal = mat3(transpose(inverse(m))) * a_normal;
	view_light_pos = vec3(v * vec4(light_pos, 1.0));
	tex_coords = a_tex_coords * vec2(1, -1);

	if (verts_type == 1)
	{
		tex_coords = rotation_matrix_for_screen_texture * tex_coords;
		tex_coords.y /= 0.72;
	}
}