#include "render/Shader.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

static std::string  readFile(const char *path)
{
    std::ifstream       file(path);
    std::stringstream   buffer;

    if (!file.is_open())
        throw std::runtime_error(std::string(path) + ": cannot open");
    buffer << file.rdbuf();
    return (buffer.str());
}

static u32  compile(u32 type, const std::string &src, const char *path)
{
    u32         shader = glCreateShader(type);
    const char  *cstr = src.c_str();
    i32         ok = 0;

    glShaderSource(shader, 1, &cstr, nullptr);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        char    log[1024];

        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        glDeleteShader(shader);
        throw std::runtime_error(std::string(path) + ": " + log);
    }
    return (shader);
}

void    Shader::load(const char *vert_path, const char *frag_path)
{
    _vert_path = vert_path;
    _frag_path = frag_path;
    u32 vert = 0;
    u32 frag = 0;
    i32 ok = 0;

    try
    {
        vert = compile(GL_VERTEX_SHADER, readFile(vert_path), vert_path);
        frag = compile(GL_FRAGMENT_SHADER, readFile(frag_path), frag_path);
    }
    catch (...)
    {
        glDeleteShader(vert);
        throw ;
    }
    if (_program)
        glDeleteProgram(_program);
    _program = glCreateProgram();
    glAttachShader(_program, vert);
    glAttachShader(_program, frag);
    glLinkProgram(_program);
    glDeleteShader(vert);
    glDeleteShader(frag);
    glGetProgramiv(_program, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        char    log[1024];

        glGetProgramInfoLog(_program, sizeof(log), nullptr, log);
        glDeleteProgram(_program);
        _program = 0;
        throw std::runtime_error(std::string(vert_path) + " + " + frag_path + ": link failed: " + log);
    }
}
