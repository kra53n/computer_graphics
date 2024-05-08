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

struct DirLight {
	 vec3 dir;

     vec3 ambient;
     vec3 diffuse;
     vec3 specular;
};

struct PointLight {
     vec3 pos;

     vec3 ambient;
     vec3 diffuse;
     vec3 specular;

     float constant;
     float linear;
     float quad;
};

struct SpotLight {
     vec3 pos;

	 vec3 dir;
	 float cut_off;
	 float outer_cut_off;

     vec3 ambient;
     vec3 diffuse;
     vec3 specular;

     float constant;
     float linear;
     float quad;
};

uniform Material material;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform int nr_dir_lights;
uniform int nr_point_lights;
uniform int nr_spot_lights;

uniform DirLight dir_lights[10];
uniform PointLight point_lights[10];
uniform SpotLight spot_lights[10];

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);

void main()
{
    vec3 norm = normalize(normal);
    vec3 view_dir = normalize(view_pos - frag_pos);
    
    vec3 res = vec3(0.0);
	for (int i = 0; i < nr_dir_lights; i++)
		res += calc_dir_light(dir_lights[i], norm, view_dir);
	for (int i = 0; i < nr_point_lights; i++)
		res += calc_point_light(point_lights[i], norm, frag_pos, view_dir);    
	for (int i = 0; i < nr_spot_lights; i++)
		res += calc_spot_light(spot_lights[i], norm, frag_pos, view_dir);    
    
    frag_col = vec4(res, 1.0);
}

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.dir);

    float diff = max(dot(normal, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, normal);
    // float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 2);

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, tex_coords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, tex_coords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, tex_coords));

    return (ambient + diffuse + specular);
}

vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.pos - frag_pos);

    float diff = max(dot(normal, light_dir), 0.0);
	
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    float distance = length(light.pos - frag_pos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quad * (distance * distance));    
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, tex_coords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, tex_coords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, tex_coords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 calc_spot_light(SpotLight light, vec3 norm, vec3 frag_pos, vec3 view_dir)
{
    vec3 light_dir = normalize(light.pos - frag_pos);
	float diff = max(dot(norm, light_dir), 0.0);

	vec3 reflect_dir = reflect(-light_dir, norm);
	float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	float distance = length(light.pos - frag_pos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quad * distance * distance);

	float theta = dot(light_dir, normalize(-light.dir));
	float epsilon = light.cut_off - light.outer_cut_off;
	float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, tex_coords));
	vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, tex_coords).rgb;
	vec3 specular = light.specular * spec * texture(texture_specular1, tex_coords).rgb;

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}