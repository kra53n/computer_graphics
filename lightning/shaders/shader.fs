#version 330 core

in vec3 frag_pos, normal, view_light_pos;

out vec4 frag_col;

uniform vec3 view_pos;
uniform vec3 obj_col, light_col;

void main()
{
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_col;

	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(view_light_pos - frag_pos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = diff * light_col;


	float specular_strength = 0.3;
	vec3 view_dir = normalize(-frag_pos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32);
	vec3 specular = specular_strength * spec * light_col;
	
	// 1
	// frag_col = vec4(frag_pos, 1.0);
	// 2
	// vec3 res = (ambient + diffuse + specular - frag_pos * 0.2) * obj_col;
	vec3 res = (ambient + diffuse + specular) * obj_col;
	frag_col = vec4(res, 1.0);
}