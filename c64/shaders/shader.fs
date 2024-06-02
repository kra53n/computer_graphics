#version 330 core

in vec3 frag_pos, normal, view_light_pos;
in vec2 tex_coords;

out vec4 frag_col;

uniform vec3 view_pos;
uniform bool is_screen_on;
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
uniform sampler2D texture_diffuse2;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform int verts_type;
const int VERTEX_TYPE_OTHER = 0;
const int VERTEX_TYPE_SCREEN = 1;

uniform int nr_dir_lights;
uniform int nr_point_lights;
uniform int nr_spot_lights;

uniform DirLight dir_lights[10];
uniform PointLight point_lights[10];
uniform SpotLight spot_lights[10];

uniform float time;
uniform float rand;

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir);
vec3 calc_point_light(PointLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);
vec3 calc_spot_light(SpotLight light, vec3 normal, vec3 frag_pos, vec3 view_dir);
vec3 some_effect(vec3 col, int effect_type);
vec3 vignette(vec3 col);

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
    
    frag_col = vec4(vignette(some_effect(res,6)), 1.0);
}

vec3 calc_dir_light(DirLight light, vec3 normal, vec3 view_dir)
{
	vec3 light_dir = normalize(-light.dir);

    float diff = max(dot(normal, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, tex_coords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, tex_coords));
    vec3 specular = light.specular * spec * vec3(texture(texture_specular1, tex_coords));

	if (is_screen_on && verts_type == 1)
	{
		ambient = light.ambient * vec3(texture(texture_diffuse2, tex_coords));
		diffuse = light.diffuse * diff * vec3(texture(texture_diffuse2, tex_coords));
	}

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

	if (is_screen_on && verts_type == 1)
	{
		ambient = light.ambient * vec3(texture(texture_diffuse2, tex_coords));
		diffuse = light.diffuse * diff * vec3(texture(texture_diffuse2, tex_coords));
	}

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

	if (is_screen_on && verts_type == 1)
	{
		ambient = light.ambient * vec3(texture(texture_diffuse2, tex_coords));
		diffuse = light.diffuse * diff * vec3(texture(texture_diffuse2, tex_coords));
	}

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}

float random(vec2 pos)
{
	return fract(sin(dot(pos, vec2(int(rand) % 50, int(rand) % 100))));
}

vec3 noise_image(vec3 tex)
{
	return mod(tex, tex) * random(vec2(rand)) * 1.4;
}

vec3 noise_pattern(vec3 col)
{
	vec2 ipos = floor(col.rb * rand * 0.3);
	ipos = floor(tex_coords * rand * 2) * ipos;
	return mod(col, vec3(random(ipos)));
}

vec3 lightning(vec3 col)
{
	vec2 noise = floor(tex_coords * fract(time) * rand);
	return mod(col, vec4(tex_coords, noise).xyz);
}

vec3 uv_shift()
{
	vec2 coords = tex_coords;
	if (coords.x > -0.1 && coords.y > -0.1)
	   coords.x += rand / 100;
	return vec3(texture(texture_diffuse2, coords));
}

vec3 rgb_shift()
{
	vec2 shift = vec2(texture(texture_diffuse2, tex_coords / mod(rand, 100)).r * 0.1, 0.0);
	float r = texture(texture_diffuse2, tex_coords + shift + 0.03).r;
	float g = texture(texture_diffuse2, tex_coords + cos(shift) + 0.02).g;
	float b = texture(texture_diffuse2, tex_coords - sin(shift) - 0.02).b;

	return vec3(r, g, b);
}

vec3 horizontal_lines(vec3 col)
{
	if (tex_coords.y < cos(rand) + 0.05 && tex_coords.y > cos(rand) && fract(time * 0.1) > -0.01 && tex_coords.x < cos(rand) + 0.6 && tex_coords.x > cos(rand) - 0.6)
	{
		return texture(texture_diffuse2, vec2(cos(time), floor(rand))).rgb;
	}
	return col;
}

vec3 vertical_lines(vec3 col)
{
	if (tex_coords.x < cos(rand) + 0.04 && tex_coords.x > cos(rand) && rand > 20)
	{
		return texture(texture_diffuse2, vec2(cos(time), floor(time * 0.05))).rgb;
	}
	return col;
}

vec3 some_effect(vec3 col, int effect_type)
{
	if (!is_screen_on ||
	    verts_type == VERTEX_TYPE_OTHER ||
		verts_type != VERTEX_TYPE_SCREEN)
		return col;
	switch (effect_type)
	{
		case 0:
			vec3 res = noise_image(col);
			res = mod(noise_pattern(col), res) * 4;
			return res;

		case 1:
			 if (rand > 90)
				return lightning(col) * rand * 0.1 % 1;
			else
				return col;

		case 2: return col + uv_shift() * 0.1;
		case 3: return rgb_shift();
		case 4: return col + horizontal_lines(col) * 0.2;
		case 5: return col + vertical_lines(col) * 0.7;
		case 6: return col + vertical_lines(lightning(rgb_shift())) * 0.7;
	}
}

vec3 vignette(vec3 col)
{
	if (!is_screen_on ||
	    verts_type == VERTEX_TYPE_OTHER ||
		verts_type != VERTEX_TYPE_SCREEN)
		return col;
	float x = tex_coords.x;
	x -= 0.5;
	x = abs(x);
	vec3 res = vec3(x, x, x);

	float y = tex_coords.y;
	y -= 0.5;
	y = abs(y);
	res = vec3(y, y, y) + res;
	// res = res * rand / 10;
	// return col / res;
	res = 1 - res * 0.9;
	return col * res;
}