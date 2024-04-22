#include "shader_program.hpp"

ShaderProgram::ShaderProgram(std::vector<Shader> shaders)
{
    _shaders = shaders;
	_compile();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(_ID);
}

void ShaderProgram::_compile()
{
    _ID = glCreateProgram();
    for (Shader shader : _shaders) shader.attach(_ID);
    _link_program();
    for (Shader shader : _shaders) shader.del();
}

void ShaderProgram::_link_program()
{
    glLinkProgram(_ID);
    int success;
    char info_log[512];
    glGetProgramiv(_ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(_ID, 512, NULL, info_log);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
    }
}

void ShaderProgram::use()
{
    glUseProgram(_ID);
}