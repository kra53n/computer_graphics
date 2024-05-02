#version 330 core

in vec3 frag_pos, normal, view_light_pos;
in vec2 tex_coords;

out vec4 frag_col;

uniform vec3 view_pos;
uniform vec3 obj_col, light_col;


struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 pos;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;
uniform Light light;

void main()
{
	vec3 ambient = light.ambient * texture(material.diffuse, tex_coords).rgb;

	vec3 norm = normalize(normal);
	vec3 light_dir = normalize(view_light_pos - frag_pos);
	float diff = max(dot(norm, light_dir), 0.0);
	vec3 diffuse = light.diffuse * diff * texture(material.diffuse, tex_coords).rgb;


	float specular_strength = 0.3;
	vec3 view_dir = normalize(-frag_pos);
	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * texture(material.specular, tex_coords).rgb;
	
	// 1
	// frag_col = vec4(frag_pos, 1.0);
	// 2
	// vec3 res = (ambient + diffuse + specular - frag_pos * 0.2) * obj_col;
	vec3 res = ambient + diffuse + specular;
	frag_col = vec4(res, 1.0);
}