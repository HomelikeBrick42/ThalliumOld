#pragma once

#include "Base.hpp"
#include "Ref.hpp"
#include "Window.hpp"

#include "Shader.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

class Renderer: public IRef {
protected:
    Renderer() = default;
public:
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&)      = delete;
    virtual ~Renderer()       = default;
public:
    virtual void Clear(const glm::vec4& color)                                                              = 0;
    virtual void BeginScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, bool depthTest) = 0;
    virtual void EndScene()                                                                                 = 0;
    virtual void
    Draw(Ref<VertexBuffer> vertexBuffer, Ref<Shader> shader, size_t first, size_t count, const glm::mat4& transform) = 0;
    virtual void
    DrawIndexed(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Shader> shader, const glm::mat4& transform) = 0;
    virtual void OnResize(uint32_t width, uint32_t height)                                                                    = 0;
    virtual void Present()                                                                                                    = 0;
public:
    virtual Ref<Shader> CreateShader(const std::string& filepath) = 0;
    virtual Ref<VertexBuffer>
    CreateVertexBuffer(const void* data, size_t size, const std::span<VertexBuffer::Element>& layout) = 0;
    virtual Ref<IndexBuffer> CreateIndexBuffer(const std::span<uint32_t>& indices)                    = 0;
public:
    virtual Ref<Window> GetWindow() = 0;
public:
    static Ref<Renderer> CreateOpenGLRenderer(Ref<Window> window);
};
