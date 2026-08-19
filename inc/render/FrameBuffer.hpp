#pragma once

#include "render/GL.hpp"
#include "math.hpp"

class   FrameBuffer
{
    public:
        FrameBuffer() {}
        ~FrameBuffer() 
        {
            if (_FBO != 0)
                glDeleteFramebuffers(1, &_FBO);
            if (_colorTex != 0)
                glDeleteTextures(1, &_colorTex);
        }

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;

        void    create(u32 width, u32 height, bool depth = true);
        void    resize(u32 width, u32 height);

        void    bind() const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
            glViewport(0, 0, _width, _height);
        }
        void    unbind() const
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        u32     id() const {return (_FBO);}
        u32     colorTexture() const {return (_colorTex);}
        u32     width() const {return (_width);}
        u32     height() const {return (_height);}
    private:
        u32     _FBO = 0;
        u32     _colorTex = 0;
        u32     _width = 0;
        u32     _height = 0;
        bool    _hasDepth = false;
};
