#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

inline void Camera::_init()
{
	set_yaw(YAW);
	set_pitch(PITCH);
	_mouse_sensetivity = MOUSE_SENSETIVITY;
	_movement_speed = SPEED;
	_zoom = ZOOM;

	set_pos(0.0f, 0.0f, 1.0f);
	_front = glm::vec3(0.0f, 0.0f, -1.0f);
	set_up(0.0f, 1.0f, 0.0f);
	_world_up = _up;
	
	_update();
}

Camera::Camera()
{
	_init();
}

Camera::Camera(glm::vec3 pos)
{
	_init();
	set_pos(pos);
}

Camera::Camera(float pos_x, float pos_y, float pos_z)
{
	_init();
	set_pos(pos_x, pos_y, pos_z);
}

Camera::Camera(glm::vec3 pos, glm::vec3 up)
{
	_init();
	set_pos(pos);
	set_up(up);
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z)
{
	_init();
	set_pos(pos_x, pos_y, pos_z);
	set_up(up_x, up_y, up_z);
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
{
	_init();
	set_pos(pos);
	set_up(up);
	_yaw = yaw;
	_pitch = pitch;
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw, float pitch)
{
	_init();
	set_pos(pos_x, pos_y, pos_z);
	set_up(up_x, up_y, up_z);
	_yaw = yaw;
	_pitch = pitch;
}

Camera::Camera(float yaw, float pitch)
{
	_init();
	_yaw = yaw;
	_pitch = pitch;
}

void Camera::set_pos(glm::vec3 val)
{
	_pos = val;
}

void Camera::set_pos(float x, float y, float z)
{
	_pos[0] = x;
	_pos[1] = y;
	_pos[2] = z;
}

void Camera::set_up(glm::vec3 val)
{
	_up = val;
}

void Camera::set_up(float x, float y, float z)
{
	_up[0] = x;
	_up[1] = y;
	_up[2] = z;
}

void Camera::set_yaw(float val)
{
	_yaw = val;
}	

void Camera::set_pitch(float val)
{
	_pitch = val;
}	

void Camera::process_keyboard(Move direction, float dt)
{
	float velocity = _movement_speed * dt;
	switch (direction)
	{
	case FORW: _pos += _front * velocity; break;
	case BACK: _pos -= _front * velocity; break;
	case LEFT: _pos -= _right * velocity; break;
	case RIGHT: _pos += _right * velocity; break;
	case UP: _pos += _up * velocity; break;
	case DOWN: _pos -= _up * velocity; break;
	}
}

void Camera::process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_pitch)
{
	x_offset *= _mouse_sensetivity;
	y_offset *= _mouse_sensetivity;

	_yaw += x_offset;
	_pitch += y_offset;

	if (constrain_pitch)
	{
		if (_pitch > 89.0f) _pitch = 89.0f;
		if (_pitch < -89.0f) _pitch = -89.0f;
	}

	_update();
}

void Camera::process_mouse_scroll(float y_offset)
{
	_zoom -= y_offset;
	if (_zoom < 1.0f) _zoom = 1.0f;
	if (_zoom > 45.0f) _zoom = 45.0f;
}

glm::mat4 Camera::get_view()
{
	return glm::lookAt(_pos, _pos + _front, _up);
}

float Camera::get_zoom()
{
	return  _zoom;
}

inline void Camera::_update()
{
	glm::vec3 new_front;
	float
		rad_yaw = glm::radians(_yaw),
		rad_pitch = glm::radians(_pitch);
	new_front.x = cos(rad_yaw) * cos(rad_pitch);
	new_front.y = sin(rad_pitch);
	new_front.z = sin(rad_yaw) * cos(rad_pitch);
	
	_front = glm::normalize(new_front);
	_right = glm::normalize(glm::cross(_front, _world_up));
	_up = glm::normalize(glm::cross(_right, _front));
}
