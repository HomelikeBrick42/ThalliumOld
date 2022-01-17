#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Texture.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLRenderer.hpp"

#include <glm/fwd.hpp>

namespace Thallium {

    class OpenGLTexture final: public Texture {
        friend class OpenGLRenderer;
    public:
        OpenGLTexture(Ref<OpenGLRenderer> renderer, const std::span<glm::u8vec4>& pixels, size_t width, size_t height);
        OpenGLTexture(Ref<OpenGLRenderer> renderer, const std::span<glm::vec4>& pixels, size_t width, size_t height);
        ~OpenGLTexture() final;
    public:
        void Bind();
        void Bind(uint8_t location);
    public:
        void SetPixels(const std::span<glm::u8vec4>& pixels, size_t width, size_t height) final;
        void SetPixels(const std::span<glm::vec4>& pixels, size_t width, size_t height) final;
        size_t GetWidth() final {
            return Width;
        }
        size_t GetHeight() final {
            return Height;
        }
    public:
        Ref<Renderer> GetRenderer() final {
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
