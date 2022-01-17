#include "Thallium/Renderer/OpenGL/OpenGLTexture.hpp"

namespace Thallium {

    OpenGLTexture::OpenGLTexture(Ref<OpenGLRenderer> renderer, const glm::u8vec4* pixels, size_t width, size_t height)
        : Renderer(renderer), Texture(UINT32_MAX), Width(width), Height(height) {
        Renderer->glGenTextures(1, &Texture);
        SetPixels(pixels, width, height);
    }

    OpenGLTexture::OpenGLTexture(Ref<OpenGLRenderer> renderer, const glm::vec4* pixels, size_t width, size_t height)
        : Renderer(renderer), Texture(UINT32_MAX), Width(width), Height(height) {
        Renderer->glGenTextures(1, &Texture);
        SetPixels(pixels, width, height);
    }

    OpenGLTexture::OpenGLTexture(Ref<OpenGLRenderer> renderer, size_t width, size_t height)
        : Renderer(renderer), Texture(UINT32_MAX), Width(width), Height(height) {
        Renderer->glGenTextures(1, &Texture);
        AllocatePixels(width, height);
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

    void OpenGLTexture::SetPixels(const glm::u8vec4* pixels, size_t width, size_t height) {
        Bind();

        Width  = width;
        Height = height;

        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Renderer->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    }

    void OpenGLTexture::SetPixels(const glm::vec4* pixels, size_t width, size_t height) {
        Bind();

        Width  = width;
        Height = height;

        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Renderer->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, pixels);
    }

    void OpenGLTexture::AllocatePixels(size_t width, size_t height) {
        Bind();

        Width  = width;
        Height = height;

        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        Renderer->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        Renderer->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, nullptr);
    }

    void OpenGLTexture::GetPixels(glm::u8vec4* outPixels) {
        Bind();
        Renderer->glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_INT, outPixels);
    }

    void OpenGLTexture::GetPixels(glm::vec4* outPixels) {
        Bind();
        Renderer->glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, outPixels);
    }

}
