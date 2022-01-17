#include "Thallium/Renderer/OpenGL/OpenGLIndexBuffer.hpp"

namespace Thallium {

    OpenGLIndexBuffer::OpenGLIndexBuffer(Ref<OpenGLRenderer> renderer, const std::span<uint32_t>& indices)
        : Renderer(renderer), IndexBuffer(UINT32_MAX), IndexCount(0) {
        Renderer->glGenBuffers(1, &IndexBuffer);
        SetIndices(indices);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        Renderer->glDeleteBuffers(1, &IndexBuffer);
    }

    void OpenGLIndexBuffer::SetIndices(const std::span<uint32_t>& indices) {
        Bind();
        Renderer->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
        IndexCount = indices.size();
    }

    void OpenGLIndexBuffer::Bind() {
        Renderer->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
    }

}
