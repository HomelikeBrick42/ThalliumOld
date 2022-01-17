#include "Thallium/Renderer/OpenGL/OpenGLFramebuffer.hpp"

#include <iostream>
#include <format>

namespace Thallium {

    OpenGLFramebuffer::OpenGLFramebuffer(Ref<OpenGLRenderer> renderer,
                                         Ref<OpenGLTexture> colorAttachment,
                                         bool hasDepthStencilAttachment)
        : Renderer(renderer)
        , Framebuffer(UINT32_MAX)
        , ColorTextureAttachment(colorAttachment)
        , DepthStencilBufferAttachment(UINT32_MAX)
        , Width(ColorTextureAttachment->GetWidth())
        , Height(ColorTextureAttachment->GetHeight()) {
        Renderer->glGenFramebuffers(1, &Framebuffer);

        if (hasDepthStencilAttachment) {
            Renderer->glGenRenderbuffers(1, &DepthStencilBufferAttachment);
        }
        Resize(Width, Height);

        Bind();
        Renderer->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTextureAttachment->Texture, 0);

        if (DepthStencilBufferAttachment != UINT32_MAX) {
            Renderer->glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, DepthStencilBufferAttachment);
        }

        if (Renderer->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << std::format("Framebuffer is not complete") << std::endl;
            std::exit(1);
        }

        Unbind();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer() {
        Renderer->glDeleteFramebuffers(1, &Framebuffer);
        if (DepthStencilBufferAttachment != UINT32_MAX) {
            Renderer->glDeleteRenderbuffers(1, &DepthStencilBufferAttachment);
        }
    }

    void OpenGLFramebuffer::Bind() {
        Renderer->glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
    }

    void OpenGLFramebuffer::Unbind() {
        Renderer->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFramebuffer::Resize(size_t width, size_t height) {
        Width  = width;
        Height = height;

        ColorTextureAttachment->AllocatePixels(width, height);

        if (DepthStencilBufferAttachment != UINT32_MAX) {
            Renderer->glBindRenderbuffer(GL_RENDERBUFFER, DepthStencilBufferAttachment);
            Renderer->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Width, Height);
        }
    }

}
