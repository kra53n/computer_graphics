#include "lights.hpp"

#include <string>

void add_lights()
{
	std::vector<ILight*> lights = {
		new DirLight(
			{ -0.2f, -1.0f, -0.3f },
			{ 0.05f, 0.05f, 0.05f },
			{ 0.4f, 0.4f, 0.4f },
			{ 0.5f, 0.5f, 0.5f }
		),
		new PointLight(
			{ 0.0f, 0.0f, 0.0f },
			{ 0.05f, 0.05f, 0.05f },
			{ 0.8f, 0.8f, 0.8f },
			{ 1.0f, 1.0f, 1.0f },
			1.0f,
			0.09f,
			0.032f
		),
		new PointLight(
			{ 2.0f, 1.0f, -3.0f },
			{ 0.15f, 0.05f, 0.05f },
			{ 0.8f, 0.5f, 0.5f },
			{ 1.0f, 1.0f, 1.0f },
			1.0f,
			0.09f,
			0.032f
		),
		new SpotLight(
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f },
			1.0f,
			0.09f,
			0.032f,
			glm::cos(glm::radians(12.5f)),
			glm::cos(glm::radians(15.0f))
		),
		new SpotLight(
			{ 0.0f, 0.0f, 0.0f },
			{ 0.1f, 0.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 0.4f, 0.4f, 1.0f },
			{ 0.4f, 0.4f, 1.0f },
			1.0f,
			0.09f,
			0.032f,
			glm::cos(glm::radians(12.5f)),
			glm::cos(glm::radians(15.0f))
		)
	};
	for (ILight* light : lights)
		add_entity(Entity::Group::Light, light);
}

void set_lights_for_shader_program(ShaderProgram* sp)
{
	std::unordered_map<unsigned, Entity*>* pairs = get_entities(Entity::Group::Light);
	int dirs = 0;
	int points = 0;
	int spots = 0;
	for (auto pair : *pairs)
	{
		ILight* light = (ILight*)pair.second;
		switch (light->type)
		{
		case Light::DirLight: light->set_for_shader_program(sp, dirs++); break;
		case Light::PointLight: light->set_for_shader_program(sp, points++); break;
		case Light::SpotLight: light->set_for_shader_program(sp, spots++); break;
		}
	}
	sp->set("nr_dir_lights", dirs);
	sp->set("nr_point_lights", points);
	sp->set("nr_spot_lights", spots);
}



DirLight::DirLight(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 spec)
{
	group = Entity::Group::Light;
	type = Light::DirLight;
	_dir = dir;
	_ambient = ambient;
	_diffuse = diffuse;
	_spec = spec;
}

void DirLight::set_for_shader_program(ShaderProgram* sp, int idx)
{
	std::string s = "dir_lights[";
	s += std::to_string(idx);
	s += "].";
	sp->set((s + "dir").c_str(), _dir);
	sp->set((s + "ambient").c_str(), _ambient);
	sp->set((s + "diffuse").c_str(), _diffuse);
	sp->set((s + "spec").c_str(), _spec);
}


PointLight::PointLight(
	glm::vec3 pos,
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 spec,
	float constant,
	float linear,
	float quadratic
) {
	group = Entity::Group::Light;
	type = Light::PointLight;
	_pos = pos;
	_ambient = ambient;
	_diffuse = diffuse;
	_spec = spec;
	_constant = constant;
	_linear = linear;
	_quadratic = quadratic;
}

void PointLight::set_for_shader_program(ShaderProgram* sp, int idx)
{
	std::string s = "point_lights[";
	s += std::to_string(idx);
	s += "].";
	sp->set((s + "pos").c_str(), _pos);
	sp->set((s + "ambient").c_str(), _ambient);
	sp->set((s + "diffuse").c_str(), _diffuse);
	sp->set((s + "spec").c_str(), _spec);
	sp->set((s + "constant").c_str(), _constant);
	sp->set((s + "linear").c_str(), _linear);
	sp->set((s + "quad").c_str(), _quadratic);
}


SpotLight::SpotLight(
	glm::vec3 pos,
	glm::vec3 dir,
	glm::vec3 ambient,
	glm::vec3 diffuse,
	glm::vec3 spec,
	float constant,
	float linear,
	float quadratic,
	float cut_off,
	float outer_cut_off
) {
	group = Entity::Group::Light;
	type = Light::SpotLight;
	_pos = pos;
	_ambient = ambient;
	_diffuse = diffuse;
	_spec = spec;
	_constant = constant;
	_linear = linear;
	_quadratic = quadratic;
	_cut_off = cut_off;
	_outer_cut_off = outer_cut_off;
}

void SpotLight::set_for_shader_program(ShaderProgram* sp, int idx)
{
	std::string s = "spot_lights[";
	s += std::to_string(idx);
	s += "].";
	sp->set((s + "pos").c_str(), _pos);
	sp->set((s + "dir").c_str(), _dir);
	sp->set((s + "ambient").c_str(), _ambient);
	sp->set((s + "diffuse").c_str(), _diffuse);
	sp->set((s + "spec").c_str(), _spec);
	sp->set((s + "constant").c_str(), _constant);
	sp->set((s + "linear").c_str(), _linear);
	sp->set((s + "quad").c_str(), _quadratic);
	sp->set((s + "cut_off").c_str(), _cut_off);
	sp->set((s + "outer_cut_off").c_str(), _outer_cut_off);
}
