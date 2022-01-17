#include "Thallium/Renderer/OpenGL/OpenGLFramebuffer.hpp"

#include <iostream>
#include <format>

namespace Thallium {

    OpenGLFramebuffer::OpenGLFramebuffer(Ref<OpenGLRenderer> renderer,
                                         Ref<OpenGLTexture> colorAttachment,
                                         bool hasDepthStencilAttachment)
        : Renderer(renderer), Framebuffer(0), ColorTextureAttachment(colorAttachment), DepthStencilBufferAttachment(0) {
        Renderer->glGenFramebuffers(1, &Framebuffer);
        Bind();

        Renderer->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ColorTextureAttachment->Texture, 0);

        if (Renderer->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << std::format("Framebuffer is not complete") << std::endl;
            std::exit(1);
        }

        Unbind();
    }

    OpenGLFramebuffer::~OpenGLFramebuffer() {
        Renderer->glDeleteFramebuffers(1, &Framebuffer);
    }

    void OpenGLFramebuffer::Bind() {
        Renderer->glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
    }

    void OpenGLFramebuffer::Unbind() {
        Renderer->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}
