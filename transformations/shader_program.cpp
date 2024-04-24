#include "shader_program.hpp"

#include <exception>

ShaderProgram::ShaderProgram(std::vector<Shader> shaders)
{
    _shaders = shaders;
	_compile();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(_ID);
}

GLuint ShaderProgram::ID()
{
    return _ID;
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

#define fill_uniform(func_name, ...) \
    func_name(glGetUniformLocation(_ID, uniform_name), __VA_ARGS__)

void ShaderProgram::set(const char* uniform_name, bool val)
{
    fill_uniform(glUniform1i, (int)val);
}

void ShaderProgram::set(const char* uniform_name, int val)
{
    fill_uniform(glUniform1i, val);
}

void ShaderProgram::set(const char* uniform_name, float val)
{
    fill_uniform(glUniform1f, val);
}

void ShaderProgram::set(const char* uniform_name, const std::vector<float>& vals)
{
    switch (vals.size())
    {
    case 1: fill_uniform(glUniform1f, vals[0]); break;
    case 2: fill_uniform(glUniform2f, vals[0], vals[1]); break;
    case 3: fill_uniform(glUniform3f, vals[0], vals[1], vals[3]); break;
    case 4: fill_uniform(glUniform4f, vals[0], vals[1], vals[3], vals[4]); break;
    default:
        throw std::exception("invalid size for uniform float vec");
    }
}