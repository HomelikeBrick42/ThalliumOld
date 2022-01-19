#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/IndexBuffer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLRenderer.hpp"

namespace Thallium {

    class OpenGLIndexBuffer final: public IndexBuffer {
        friend class OpenGLRenderer;
    public:
        OpenGLIndexBuffer(Ref<OpenGLRenderer> renderer, const std::span<uint32_t>& indices);
        ~OpenGLIndexBuffer() final;
    public:
        void SetIndices(const std::span<uint32_t>& indices) final;
        size_t GetIndexCount() const final {
            return IndexCount;
        }
        void Bind();
    public:
        Ref<Renderer> GetRenderer() const final {
            return Renderer;
        }
    private:
        Ref<OpenGLRenderer> Renderer;
        uint32_t IndexBuffer;
        size_t IndexCount;
    };

}
