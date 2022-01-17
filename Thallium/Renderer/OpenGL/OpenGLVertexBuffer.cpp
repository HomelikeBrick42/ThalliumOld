#include "Thallium/Renderer/OpenGL/OpenGLVertexBuffer.hpp"

#include <iostream>
#include <format>

namespace Thallium {

    OpenGLVertexBuffer::OpenGLVertexBuffer(Ref<OpenGLRenderer> renderer,
                                           const void* data,
                                           size_t size,
                                           const std::span<Element>& layout)
        : Renderer(renderer), VertexArray(0), VertexBuffer(0) {
        Renderer->glGenVertexArrays(1, &VertexArray);
        Renderer->glGenBuffers(1, &VertexBuffer);
        SetData(data, size);
        SetLayout(layout);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {
        Renderer->glDeleteBuffers(1, &VertexBuffer);
        Renderer->glDeleteVertexArrays(1, &VertexArray);
    }

    void OpenGLVertexBuffer::SetData(const void* data, size_t size) {
        Bind();
        Renderer->glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    static int32_t GetElementTypeCount(VertexBuffer::ElementType type) {
        switch (type) {
            case VertexBuffer::ElementType::Float:
                return 1;
            case VertexBuffer::ElementType::Float2:
                return 2;
            case VertexBuffer::ElementType::Float3:
                return 3;
            case VertexBuffer::ElementType::Float4:
                return 4;
        }
        std::cerr << std::format("Invalid type for GetElementTypeCount: {}", static_cast<int>(type)) << std::endl;
        std::exit(1);
    }

    static uint32_t ElementTypeToGLType(VertexBuffer::ElementType type) {
        switch (type) {
            case VertexBuffer::ElementType::Float:
            case VertexBuffer::ElementType::Float2:
            case VertexBuffer::ElementType::Float3:
            case VertexBuffer::ElementType::Float4:
                return GL_FLOAT;
        }
        std::cerr << std::format("Invalid type for ElementTypeToGLType: {}", static_cast<int>(type)) << std::endl;
        std::exit(1);
    }

    void OpenGLVertexBuffer::SetLayout(const std::span<Element>& layout) {
        Bind();
        uint32_t i = 0;
        for (auto& element : layout) {
            Renderer->glEnableVertexAttribArray(i);
            Renderer->glVertexAttribPointer(i,
                                            GetElementTypeCount(element.Type),
                                            ElementTypeToGLType(element.Type),
                                            element.Normalized,
                                            static_cast<uint32_t>(element.Stride),
                                            reinterpret_cast<const void*>(element.Offset));
            i++;
        }
    }

    void OpenGLVertexBuffer::Bind() {
        Renderer->glBindVertexArray(VertexArray);
        Renderer->glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    }

}
