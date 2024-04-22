#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader_program.hpp"


GLFWwindow* create_window()
{
	GLFWwindow* window = glfwCreateWindow(800, 600, "window", NULL, NULL);
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
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

	load_glad();

	//

	ShaderProgram shader_program({
		Shader("shaders/shader.vs"),
		Shader("shaders/shader.fs"),
	});

	//

	//float vertices[] = {
	//	 .0f,         .5f, .0f,  1.f, .0f, .0f,
	//	 0.354f,  -0.354f, .0f,  .0f, 1.f, .0f,
	//	 -0.354f, -0.354f, .0f,  .0f, .0f, 1.f
	//};

	unsigned VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//

	shader_program.use();

	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();
		// 1
		//float x = sin(time * M_PI);
		//float y = sin(time * M_PI / 3);
		//float z = sin(time * M_PI / 3 * 2);
		// 2
		//float v = abs(sin(time/4));
		//float x = v <= 1. / 3 ? v * 3 : 0;
		//float y = v > 1. / 3 and v <= 2. / 3 ? (v - 1. / 3) * 3 : 0;
		//float z = v > 2. / 3 ? (v - 2. / 3) * 3 : 0;
		// 3
		//float v = abs(sin(time/4));
		//float x = v <= 1. / 3 ? v * 3 : 0;
		//float y = v > 1. / 3 and v <= 2. / 3 ? (v - 1. / 3) * 3 : 0;
		//float z = v > 2. / 3 ? (v - 2. / 3) * 3 : 0;
		//x = x == 0 ? (0.8*y + 0.2*z) / 8 : x;
		//y = y == 0 ? (0.2*x + 0.8*z) / 12 : y;
		//z = z == 0 ? (0.8*x + 0.2*y) / 6 : z;
		// 4 - finale
		float x = sin(time);
		float y = sin(time + M_PI / 3);
		float z = sin(time - M_PI / 3);
		// it is also good idea to play with x y z in vertices
		float vertices[] = {
				 .0f,         .5f, .0f,  x, y, z,
				 0.354f,  -0.354f, .0f,  z, x, y,
				 -0.354f, -0.354f, .0f,  y, z, x
		};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		shader_program.set("time", (float)glfwGetTime());

		process_input(window);

		glClearColor(.2f, .3f, .3f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
}