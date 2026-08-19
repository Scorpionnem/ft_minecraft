#include "render/FrameBuffer.hpp"

void    FrameBuffer::create(u32 width, u32 height, bool depth)
{
    _width = width;
    _height = height;
    _hasDepth = depth;

    glGenFramebuffers(1, &_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

    glGenTextures(1, &_colorTex);
    glBindTexture(GL_TEXTURE_2D, _colorTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorTex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Framebuffer: incomplete");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void    FrameBuffer::resize(u32 width, u32 height)
{
    if (width == _width && height == _height)
        return ;
    if (_FBO != 0)
        glDeleteFramebuffers(1, &_FBO);
    if (_colorTex != 0)
        glDeleteTextures(1, &_colorTex);
    create(width, height, _hasDepth);
}
