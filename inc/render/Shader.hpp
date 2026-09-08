#pragma once

#include "math.hpp"
#include "render/GL.hpp"

class   Shader
{
    public:
        Shader() {}
        ~Shader() {}

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void    load(const char *vert_path, const char *frag_path);
        void    unload()
        {
            if (_program)
            {
                glDeleteProgram(_program);
                _program = 0;
            }
        }
        void    reload()
        {
            load(_vert_path.c_str(), _frag_path.c_str());
        }
        void    bind() const
        {
            glUseProgram(_program);
        }

        void    setInt(const char *name, i32 value) const
        {
            glUniform1i(glGetUniformLocation(_program, name), value);
        }
        void    setFloat(const char *name, float value) const
        {
            glUniform1f(glGetUniformLocation(_program, name), value);
        }
        void    setVec3(const char *name, const vec3f& v) const
        {
            glUniform3fv(glGetUniformLocation(_program, name), 1, v.data);
        }
        void    setVec2(const char *name, const vec2f& v) const
        {
            glUniform2fv(glGetUniformLocation(_program, name), 1, v.data);
        }
        void    setVec4(const char *name, const vec4f& v) const
        {
            glUniform4fv(glGetUniformLocation(_program, name), 1, v.data);
        }
        void    setMat4(const char *name, const mat4f& m) const
        {
            glUniformMatrix4fv(glGetUniformLocation(_program, name), 1, GL_FALSE, m.data);
        }
    private:
        u32 _program = 0;
        std::string _frag_path;
        std::string _vert_path;
};
