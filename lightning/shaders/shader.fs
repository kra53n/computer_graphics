#version 330 core

in vec3 frag_pos;
in vec3 normal;

out vec4 frag_col;

uniform vec3 light_pos;
uniform vec3 obj_col, light_col;

void main()
{
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_col;

	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(light_pos - frag_pos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * light_col;
	
	// vec3 res = (frag_pos * 0.2 - ambient + diffuse ) * obj_col;
	vec3 res = (ambient + diffuse) * obj_col;
	frag_col = vec4(res, 1.0);
	// frag_col = vec4(frag_pos, 1.0);
}