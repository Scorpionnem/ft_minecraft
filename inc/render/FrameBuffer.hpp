#pragma once

#include "render/GL.hpp"
#include "render/Texture.hpp"
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
            if (_depthTex != 0)
                glDeleteTextures(1, &_depthTex);
        }

        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer& operator=(const FrameBuffer&) = delete;

        void    create(u32 width, u32 height);
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
        static void	clear()
        {
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        u32     id() const {return (_FBO);}
        void	bindColor(u32 unit) {Texture::bind(_colorTex, unit);}
		void	bindDepth(u32 unit) {Texture::bind(_depthTex, unit);}
		u32     colorTexture() const {return (_colorTex);}
        u32     depthTexture() const {return (_depthTex);}
        u32     width() const {return (_width);}
        u32     height() const {return (_height);}
    private:
        u32     _FBO = 0;
        u32     _colorTex = 0;
        u32		_depthTex = 0;
        u32     _width = 0;
        u32     _height = 0;
};
