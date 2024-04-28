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
    _activate_textures();
}

void ShaderProgram::del()
{
    glDeleteProgram(_ID);
}

void ShaderProgram::recompile()
{
    del();
    for (Shader& shader : _shaders) shader.recompile();
    _compile();

    use();
    for (int i = 0; i < _textures.size(); i++)
        set(_textures[i].get_uniform_name(), i);
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

//void ShaderProgram::set(const char* uniform_name, const std::vector<float>& vals)
//{
//    switch (vals.size())
//    {
//    case 1: fill_uniform(glUniform1f, vals[0]); break;
//    case 2: fill_uniform(glUniform2f, vals[0], vals[1]); break;
//    case 3: fill_uniform(glUniform3f, vals[0], vals[1], vals[2]); break;
//    case 4: fill_uniform(glUniform4f, vals[0], vals[1], vals[2], vals[3]); break;
//    default:
//        throw std::exception("invalid size for uniform float vec");
//    }
//}

void ShaderProgram::set(const char* uniform_name, const glm::vec3& val)
{
    fill_uniform(glUniform3f, val[0], val[1], val[2]);
}

void ShaderProgram::set(const char* uniform_name, const glm::mat4& val)
{
    fill_uniform(glUniformMatrix4fv, 1, GL_FALSE, &val[0][0]);
}

void ShaderProgram::set(Texture texture)
{
    set(texture.get_uniform_name(), (int)_textures.size());
    _textures.push_back(texture);
}

void ShaderProgram::set(const std::vector<Texture>& textures)
{
    for (Texture t : textures) set(t);
}

void ShaderProgram::_activate_textures()
{
    for (int i = 0; i < _textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, _textures[i].ID());
    }
}