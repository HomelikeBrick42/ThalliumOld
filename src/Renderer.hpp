#pragma once

#include "Base.hpp"
#include "Ref.hpp"
#include "Window.hpp"

#include "Shader.hpp"
#include "VertexBuffer.hpp"

class OpenGLRenderer;

class Renderer: public IRef {
protected:
    Renderer() = default;
public:
    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&)      = delete;
    virtual ~Renderer()       = default;
public:
    virtual void Draw(Ref<VertexBuffer> vertexBuffer, Ref<Shader> shader, size_t first, size_t count) = 0;
    virtual void OnResize(uint32_t width, uint32_t height)                                            = 0;
    virtual void Present()                                                                            = 0;
public:
    virtual Ref<Shader> CreateShader(const std::string& filepath) = 0;
    virtual Ref<VertexBuffer>
    CreateVertexBuffer(const void* data, size_t size, const std::span<VertexBuffer::Element>& layout) = 0;
public:
    virtual Ref<Window> GetWindow() = 0;
public:
    static Ref<OpenGLRenderer> CreateOpenGLRenderer(Ref<Window> window);
};
