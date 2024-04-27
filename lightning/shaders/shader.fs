#version 330 core

in vec2 tex_coord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

vec4 j_col;

void main()
{
	j_col = vec4(1.1, 1.0, 1.0, 1.0);
	FragColor = mix(
		texture(texture0, tex_coord),
		texture(texture1, tex_coord),
		1./3
	) * j_col;
}