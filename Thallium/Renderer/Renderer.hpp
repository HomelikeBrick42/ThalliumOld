#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Core/Ref.hpp"
#include "Thallium/Core/Window.hpp"
#include "Thallium/Core/Transform.hpp"

#include "Thallium/Renderer/Material.hpp"
#include "Thallium/Renderer/Shader.hpp"
#include "Thallium/Renderer/VertexBuffer.hpp"
#include "Thallium/Renderer/IndexBuffer.hpp"
#include "Thallium/Renderer/Texture.hpp"
#include "Thallium/Renderer/Framebuffer.hpp"

namespace Thallium {

    class Renderer: public IRef {
    protected:
        Renderer() = default;
    public:
        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&)      = delete;
        virtual ~Renderer()       = default;
    public:
        virtual void Clear(const glm::vec4& color)                      = 0;
        virtual void BeginScene(const glm::mat4& cameraTransform,
                                const glm::mat4& projectionMatrix,
                                bool depthTest,
                                Ref<Framebuffer> framebuffer = nullptr) = 0;
        virtual void EndScene()                                         = 0;
        virtual void Draw(Ref<VertexBuffer> vertexBuffer,
                          Ref<Shader> shader,
                          size_t first,
                          size_t count,
                          const Transform& transform,
                          const Material& material)                     = 0;
        virtual void DrawIndexed(Ref<VertexBuffer> vertexBuffer,
                                 Ref<IndexBuffer> indexBuffer,
                                 Ref<Shader> shader,
                                 const Transform& transform,
                                 const Material& material)              = 0;
        virtual void OnResize(uint32_t width, uint32_t height)          = 0;
        virtual void Present()                                          = 0;
    public:
        virtual Ref<Shader> CreateShader(const std::string& filepath) = 0;
        virtual Ref<VertexBuffer>
        CreateVertexBuffer(const void* data, size_t size, const std::span<VertexBuffer::Element>& layout)        = 0;
        virtual Ref<IndexBuffer> CreateIndexBuffer(const std::span<uint32_t>& indices)                           = 0;
        virtual Ref<Texture> CreateTexture(const glm::u8vec4* pixels, size_t width, size_t height)               = 0;
        virtual Ref<Texture> CreateTexture(const glm::vec4* pixels, size_t width, size_t height)                 = 0;
        virtual Ref<Texture> CreateTexture(size_t width, size_t height)                                          = 0;
        virtual Ref<Framebuffer> CreateFramebuffer(Ref<Texture> colorAttachment, bool hasDepthStencilAttachment) = 0;
    public:
        virtual Ref<Window> GetWindow() = 0;
    public:
        static Ref<Renderer> CreateOpenGLRenderer(Ref<Window> window);
    };

}
