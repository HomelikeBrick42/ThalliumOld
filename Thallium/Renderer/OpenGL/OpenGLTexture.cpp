#include "Thallium/Renderer/OpenGL/OpenGLTexture.hpp"

namespace Thallium {

    OpenGLTexture::OpenGLTexture(Ref<OpenGLRenderer> renderer, const std::span<glm::u8vec4>& pixels, size_t width, size_t height)
        : Renderer(renderer), Texture(0), Width(width), Height(height) {
        Renderer->glGenTextures(1, &Texture);
        SetPixels(pixels, width, height);
    }

    OpenGLTexture::OpenGLTexture(Ref<OpenGLRenderer> renderer, const std::span<glm::vec4>& pixels, size_t width, size_t height)
        : Renderer(renderer), Texture(0), Width(width), Height(height) {
        Renderer->glGenTextures(1, &Texture);
        SetPixels(pixels, width, height);
    }

    OpenGLTexture::~OpenGLTexture() {
        Renderer->glDeleteTextures(1, &Texture);
    }

    void OpenGLTexture::Bind() {
        Renderer->glBindTexture(GL_TEXTURE_2D, Texture);
    }

    void OpenGLTexture::Bind(uint8_t location) {
        Renderer->glActiveTexture(GL_TEXTURE0 + location);
        Bind();
    }

    void OpenGLTexture::SetPixels(const std::span<glm::u8vec4>& pixels, size_t width, size_t height) {
        Bind();

        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Renderer->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
    }

    void OpenGLTexture::SetPixels(const std::span<glm::vec4>& pixels, size_t width, size_t height) {
        Bind();

        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Renderer->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, pixels.data());
    }
}
