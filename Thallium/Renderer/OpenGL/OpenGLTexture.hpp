#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Texture.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLRenderer.hpp"

#include <glm/fwd.hpp>

namespace Thallium {

    class OpenGLTexture final: public Texture {
        friend class OpenGLRenderer;
        friend class OpenGLFramebuffer;
    public:
        OpenGLTexture(Ref<OpenGLRenderer> renderer, const glm::u8vec4* pixels, size_t width, size_t height);
        OpenGLTexture(Ref<OpenGLRenderer> renderer, const glm::vec4* pixels, size_t width, size_t height);
        OpenGLTexture(Ref<OpenGLRenderer> renderer, size_t width, size_t height);
        ~OpenGLTexture() final;
    public:
        void Bind();
        void Bind(uint8_t location);
    public:
        void SetPixels(const glm::u8vec4* pixels, size_t width, size_t height) final;
        void SetPixels(const glm::vec4* pixels, size_t width, size_t height) final;
        void AllocatePixels(size_t width, size_t height) final;
        void GetPixels(glm::u8vec4* outPixels) final;
        void GetPixels(glm::vec4* outPixels) final;
        size_t GetWidth() const final {
            return Width;
        }
        size_t GetHeight() const final {
            return Height;
        }
    public:
        Ref<Renderer> GetRenderer() const final {
            return Renderer;
        }
    private:
    private:
        Ref<OpenGLRenderer> Renderer;
        uint32_t Texture;
        size_t Width;
        size_t Height;
    };

}
