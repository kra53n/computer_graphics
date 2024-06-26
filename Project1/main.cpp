#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

const char* VERTEX_SHADER_SOURCE =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* FRAGMENT_SHADER_SOURCE =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

const char* FRAGMENT_SHADER_SOURCE1 =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
	"}\n\0";

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

	unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &VERTEX_SHADER_SOURCE, NULL);
	glCompileShader(vertex_shader);

	int success;
	char info_log[512];
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);

	if (not success)
	{
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", info_log);
	}

	//

	//

	unsigned int fragment_shader;
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &FRAGMENT_SHADER_SOURCE, NULL);
	glCompileShader(fragment_shader);

	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);

	if (not success)
	{
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", info_log);
	}

	//

	//

	unsigned int fragment_shader1;
	fragment_shader1 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader1, 1, &FRAGMENT_SHADER_SOURCE1, NULL);
	glCompileShader(fragment_shader1);

	glGetShaderiv(fragment_shader1, GL_COMPILE_STATUS, &success);

	if (not success)
	{
		glGetShaderInfoLog(fragment_shader1, 512, NULL, info_log);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", info_log);
	}

	//

	//

	unsigned shader_program = glCreateProgram();
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (not success)
	{
		glGetProgramInfoLog(shader_program, 512, NULL, info_log);
		printf("ERROR::SHADER::PROGRAM::LINKLING_FALIED\n%s", info_log);
	}

	//

	//

	unsigned shader_program1 = glCreateProgram();
	glAttachShader(shader_program1, vertex_shader);
	glAttachShader(shader_program1, fragment_shader1);
	glLinkProgram(shader_program1);

	glGetProgramiv(shader_program1, GL_LINK_STATUS, &success);
	if (not success)
	{
		glGetProgramInfoLog(shader_program1, 512, NULL, info_log);
		printf("ERROR::SHADER::PROGRAM::LINKLING_FALIED\n%s", info_log);
	}


	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glDeleteShader(fragment_shader1);

	//

	//

	float vertices[] = {
		// .5,  .5, .0, // top right
		// .5, -.5, .0, // bottom right
		//-.5, -.5, .0, // bottom left
		//-.5,  .5, .0, // top left

		0, 0,
		0, 1,
		1, 0,
	};
	float vertices1[] = {
		0, 0,
		1, 0,
		0, -1,
	};
	unsigned indexes[] = {
		0, 1, 3, // the 1st triangle
		1, 2, 3, // the 2nd triangle
	};

	//unsigned VBO, VAO, EBO;
	unsigned VBOs[2], VAOs[2];
	// VBO - Vertex Buffer Objects
	// VAO - Vertex Array Objects
	// Element Buffer Objects

	glGenBuffers(2, VBOs);
	glGenVertexArrays(2, VAOs);
	//glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);


	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glBindVertexArray(0);

	//

	while (!glfwWindowShouldClose(window))
	{
		process_input(window);

		glClearColor(.2, .3, .3, 1.);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glUseProgram(shader_program1);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	//glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader_program);
	glDeleteProgram(shader_program1);

	glfwTerminate();
}