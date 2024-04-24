#version 330 core

in vec4 out_col;
in vec2 tex_coord;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	FragColor = mix(
		texture(texture0, tex_coord),
		texture(texture1, tex_coord),
		0.2
	) * out_col;
}