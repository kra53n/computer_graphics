#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "camera.hpp"
#include "entity.hpp"
#include "model.hpp"
#include "lights.hpp"
#include "texture.hpp"
#include "shader_program.hpp"

#define WIN_WDT 800
#define WIN_HGT 600

ShaderProgram* g_shader_program_ptr = NULL;
Camera g_camera;

bool g_first_mouse = true;
bool g_is_c64_screen_on = false;
bool g_prev_c64_screen_presseing_state = false;

float g_last_x = WIN_WDT / 2;
float g_last_y = WIN_WDT / 2;
float g_dt = 0.0f; // deltatime
float g_lf = 0.0f; // last frame


GLFWwindow* create_window()
{
	GLFWwindow* window = glfwCreateWindow(WIN_WDT, WIN_HGT, "window", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window");
		glfwTerminate();
		exit(-1);
	}
}

void load_glad()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to init GLAD");
		exit(-1);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int wdt, int hgt)
{
	glViewport(0, 0, wdt, hgt);
}

void process_input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) g_shader_program_ptr->recompile();

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) g_camera.process_keyboard(Camera::FORW, g_dt);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) g_camera.process_keyboard(Camera::BACK, g_dt);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) g_camera.process_keyboard(Camera::LEFT, g_dt);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) g_camera.process_keyboard(Camera::RIGHT, g_dt);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) g_camera.process_keyboard(Camera::UP, g_dt);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) g_camera.process_keyboard(Camera::DOWN, g_dt);

	if (!g_prev_c64_screen_presseing_state and glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		g_is_c64_screen_on = g_is_c64_screen_on ^ true;
		g_prev_c64_screen_presseing_state = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
		g_prev_c64_screen_presseing_state = false;
}

void mouse_callback(GLFWwindow* window, double x_pos_in, double y_pos_in)
{
	float
		x_pos = (float)x_pos_in,
		y_pos = (float)y_pos_in;

	if (g_first_mouse)
	{
		g_last_x = x_pos;
		g_last_y = y_pos;
		g_first_mouse = false;
	}

	float
		x_offset = x_pos - g_last_x,
		y_offset = g_last_y - y_pos;

	g_last_x = x_pos;
	g_last_y = y_pos;
	
	g_camera.process_mouse_movement(x_offset, y_offset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	g_camera.process_mouse_scroll(float(y_offset));
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	auto window = create_window();
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	load_glad();

	//

	glEnable(GL_DEPTH_TEST);

	//

	ShaderProgram shader_program({
		Shader("shaders/shader.vs"),
		Shader("shaders/shader.fs"),
	});
	g_shader_program_ptr = &shader_program;

	//

	add_lights();
	DirLight* light = (DirLight*)get_entity(0);

	//

	Model c64_model("assets/objs/c64/source/", "c64.obj");
	load_texture_and_add_as_entity("assets/goku.png", "goku");

	//
	glm::vec3 light_pos = { -1.0f, -1.0f, -1.0f };

	shader_program.use();

	while (!glfwWindowShouldClose(window))
	{
		float curr_frame = (float)glfwGetTime();
		g_dt = curr_frame - g_lf;
		g_lf = curr_frame;

		process_input(window);

		glClearColor(0.05f, 0.02f, 0.02f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader_program.use();
		shader_program.set("time", (float)curr_frame);
		shader_program.set("rand", (float)(rand() % 100));

		shader_program.set("obj_col", { 1.0f, 0.5f, 0.31f });
		shader_program.set("light_col", { 1.0f, 1.0f, 1.0f });
		shader_program.set("light_pos", light_pos);
		shader_program.set("view_pos", g_camera.get_pos());
		shader_program.set("material.shininess", 32.0f);
		shader_program.set("is_screen_on", g_is_c64_screen_on);

		set_lights_for_shader_program(&shader_program);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		int win_wdt;
		int win_hgt;
		glfwGetFramebufferSize(window, &win_wdt, &win_hgt);
		projection = glm::perspective(glm::radians(g_camera.get_zoom()), (float)win_wdt / (float)win_hgt, 0.1f, 100.0f);
		view = g_camera.get_view();
		glm::mat4 pv = projection * view;
		glm::mat4 vm = view * model;
		shader_program.set("v", g_camera.get_view());
		shader_program.set("m", model);
		shader_program.set("vm", vm);
		shader_program.set("pv", pv);

		c64_model.draw(&shader_program);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
}
