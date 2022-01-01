#pragma once

#include "Base.hpp"
#include "IndexBuffer.hpp"
#include "OpenGLRenderer.hpp"

class OpenGLIndexBuffer final: public IndexBuffer {
    friend class OpenGLRenderer;
public:
    OpenGLIndexBuffer(Ref<OpenGLRenderer> renderer, const std::span<uint32_t>& indices);
    ~OpenGLIndexBuffer() final;
public:
    void SetIndices(const std::span<uint32_t>& indices) final;
    size_t GetIndexCount() final {
        return IndexCount;
    }
    void Bind();
public:
    Ref<Renderer> GetRenderer() final {
        return Renderer;
    }
private:
    Ref<OpenGLRenderer> Renderer;
    uint32_t IndexBuffer;
    size_t IndexCount;
};
