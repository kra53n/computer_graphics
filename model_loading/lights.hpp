#pragma once

#include "entity.hpp"

#include <glm/glm.hpp>

#include "shader_program.hpp"

void add_lights();
void set_lights_for_shader_program(ShaderProgram* sp);

enum class Light
{
	DirLight,
	PointLight,
	SpotLight,
};

struct ILight : Entity
{
	virtual void set_for_shader_program(ShaderProgram* sp, int idx) = 0;
	Light type;
};

class DirLight : public ILight
{
	glm::vec3
		_dir,
		_ambient,
		_diffuse,
		_spec;

public:
	DirLight(glm::vec3 dir, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 spec);
	void set_for_shader_program(ShaderProgram* sp, int idx);
};

class PointLight : public ILight
{
	glm::vec3
		_pos,
		_ambient,
		_diffuse,
		_spec;
    
    float
		_constant,
		_linear,
		_quadratic;

public:
	PointLight(
		glm::vec3 pos,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 spec,
		float constant,
		float linear,
		float quadratic
	);
	void set_for_shader_program(ShaderProgram* sp, int idx);
};

class SpotLight : public ILight
{
	glm::vec3
		_pos,
		_dir,
		_ambient,
		_diffuse,
		_spec;
    
    float
		_constant,
		_linear,
		_quadratic,
		_cut_off,
		_outer_cut_off;

public:
	SpotLight(
		glm::vec3 pos,
		glm::vec3 dir,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 spec,
		float constant,
		float linear,
		float quadratic,
		float cuf_off,
		float outer_cuf_off
	);
	void set_for_shader_program(ShaderProgram* sp, int idx);
};
