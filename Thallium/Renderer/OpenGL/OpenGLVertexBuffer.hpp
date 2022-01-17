#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/VertexBuffer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLRenderer.hpp"

namespace Thallium {

    class OpenGLVertexBuffer final: public VertexBuffer {
        friend class OpenGLRenderer;
    public:
        OpenGLVertexBuffer(Ref<OpenGLRenderer> renderer, const void* data, size_t size, const std::span<Element>& layout);
        ~OpenGLVertexBuffer() final;
    public:
        void SetData(const void* data, size_t size) final;
        void SetLayout(const std::span<Element>& layout) final;
        void Bind();
    public:
        Ref<Renderer> GetRenderer() final {
            return Renderer;
        }
    private:
        Ref<OpenGLRenderer> Renderer;
        uint32_t VertexArray;
        uint32_t VertexBuffer;
    };

}
