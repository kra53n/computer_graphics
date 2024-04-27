#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Camera
{
public:
	const float
		YAW = -90.0f,
		PITCH = 0.0f,
		SPEED = 2.5f,
		MOUSE_SENSETIVITY = 0.1f,
		ZOOM = 45.0f;

	enum Move
	{
		FORW,
		BACK,
		LEFT,
		RIGHT,
		UP,
		DOWN,
	};

private:
	float
		_yaw,
		_pitch,
		_movement_speed,
		_mouse_sensetivity,
		_zoom;

	glm::vec3
		_pos,
		_front,
		_up,
		_right,
		_world_up;

public:
	Camera();

	Camera(glm::vec3 pos);
	Camera(float pos_x, float pos_y, float pos_z);

	Camera(glm::vec3 pos, glm::vec3 up);
	Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z);

	Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch);
	Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch);

	Camera(float yaw, float pitch);

	void set_pos(glm::vec3 val);
	void set_pos(float x, float y, float z);
	void set_up(glm::vec3 val);
	void set_up(float x, float y, float z);
	void set_yaw(float val);
	void set_pitch(float val);

	void process_keyboard(Move direction, float dt);
	void process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_pitch = true);
	void process_mouse_scroll(float y_offset);

	glm::mat4 get_view();

private:
	inline void _init();
	inline void _update();
};
