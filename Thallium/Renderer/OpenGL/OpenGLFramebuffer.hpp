#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Framebuffer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLRenderer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLTexture.hpp"

namespace Thallium {

    class OpenGLFramebuffer final: public Framebuffer {
        friend class OpenGLRenderer;
    public:
        OpenGLFramebuffer(Ref<OpenGLRenderer> renderer, Ref<OpenGLTexture> colorAttachment, bool hasDepthStencilAttachment);
        ~OpenGLFramebuffer() final;
    public:
        void Bind();
        void Unbind();
    public:
        void Resize(size_t width, size_t height) final;
        void CopyInto(Ref<Framebuffer> dest) final;
        Ref<Texture> GetColorAttachment() final {
            return ColorTextureAttachment;
        }
        virtual size_t GetWidth() final {
            return Width;
        }
        virtual size_t GetHeight() final {
            return Height;
        }
    public:
        Ref<Renderer> GetRenderer() final {
            return Renderer;
        }
    private:
        Ref<OpenGLRenderer> Renderer;
        uint32_t Framebuffer;
        Ref<OpenGLTexture> ColorTextureAttachment;
        uint32_t DepthStencilBufferAttachment;
        size_t Width;
        size_t Height;
    };

}
