#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Core/Ref.hpp"

#include <span>

namespace Thallium {

    class Renderer;

    class VertexBuffer: public IRef {
    public:
        enum class ElementType {
            Float,
            Float2,
            Float3,
            Float4,
        };
        struct Element {
            ElementType Type;
            size_t Offset;
            size_t Stride;
            bool Normalized;
        };
    protected:
        VertexBuffer() = default;
    public:
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&)      = delete;
        virtual ~VertexBuffer()           = default;
    public:
        virtual void SetData(const void* data, size_t size)      = 0;
        virtual void SetLayout(const std::span<Element>& layout) = 0;
    public:
        virtual Ref<Renderer> GetRenderer() const = 0;
    };

}
