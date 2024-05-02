#include "shader.hpp"

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char* shader_path)
{
    _path = shader_path;
    _compile();
}

Shader::~Shader()
{
    del();
}

void Shader::attach(GLuint shader_program_id)
{
    glAttachShader(shader_program_id, _ID);
}

void Shader::recompile()
{
    del();
    _compile();
}

void Shader::del()
{
    glDeleteShader(_ID);
}

GLenum Shader::_get_shader_type()
{
    char* shader_letter_ptr = strchr((char*)_path, '.') + 1;
    GLenum type;
    switch (*shader_letter_ptr)
    {
    case 'v': type = GL_VERTEX_SHADER; break;
    case 'f': type = GL_FRAGMENT_SHADER; break;
    default:
        std::string error_message = "shader processing was not implemented for ";
        error_message += _path;
        throw std::exception(error_message.c_str());
    }
    return type;
}

const char* Shader::_get_shader_type_as_str()
{
    switch (_get_shader_type())
    {
    case GL_VERTEX_SHADER: return "VERTEX";
    case GL_FRAGMENT_SHADER: return "FRAGMENT";
    }
}

std::string Shader::_read_file()
{
    std::string content;
    std::ifstream file_stream;

    file_stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file_stream.open(_path);

        std::stringstream string_stream;
        string_stream << file_stream.rdbuf();

        file_stream.close();

        content = string_stream.str();
    }
    catch (std::ifstream::failure e) {
        printf("ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
    }

    return content;
}

void Shader::_compile()
{
    _ID = glCreateShader(_get_shader_type());
    std::string content = _read_file();
    const char* content_c_str = content.c_str();
    glShaderSource(_ID, 1, &content_c_str, NULL);
    glCompileShader(_ID);

    int success;
    char info_log[512];
    glGetShaderiv(_ID, GL_COMPILE_STATUS, &success);
    if (not success)
    {
        glGetShaderInfoLog(_ID, 512, NULL, info_log);
        printf(
            "ERROR::SHADER::%s::%s::COMPILATION_FAILED\n%s\n",
            _get_shader_type_as_str(),
			_path,
            info_log
        );
    }
}
