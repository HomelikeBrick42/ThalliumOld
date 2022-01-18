#include "Thallium/Renderer/OpenGL/OpenGLRenderer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLShader.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLVertexBuffer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLIndexBuffer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLTexture.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLFramebuffer.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <format>
#include <ranges>

namespace Thallium {

    Ref<Shader> OpenGLRenderer::CreateShader(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << std::format("Cannot open file '{}'", filepath) << std::endl;
            std::exit(1);
        }

        std::string vertexSource;
        std::string fragmentSource;

        enum class ShaderType {
            Unknown,
            Shared,
            Vertex,
            Fragment,
        };

        ShaderType type = ShaderType::Unknown;

        std::string line;
        while (std::getline(file, line)) {
            line.erase(0, line.find_first_not_of(" \r\n\t"));
            if (line.starts_with("#shader shared")) {
                type = ShaderType::Shared;
            } else if (line.starts_with("#shader vertex")) {
                type = ShaderType::Vertex;
            } else if (line.starts_with("#shader fragment")) {
                type = ShaderType::Fragment;
            } else if (line.starts_with("#shader")) {
                std::cerr << std::format("Unknown shader directive '{}'", line) << std::endl;
                std::exit(1);
            } else {
                switch (type) {
                    case ShaderType::Unknown: {
                        std::cerr << "Expected #shader directive" << std::endl;
                        std::exit(1);
                    } break;

                    case ShaderType::Shared: {
                        vertexSource += line;
                        vertexSource += '\n';
                        fragmentSource += line;
                        fragmentSource += '\n';
                    } break;

                    case ShaderType::Vertex: {
                        vertexSource += line;
                        vertexSource += '\n';
                    } break;

                    case ShaderType::Fragment: {
                        fragmentSource += line;
                        fragmentSource += '\n';
                    } break;
                }
            }
        }

        return Ref<OpenGLShader>::Create(this, vertexSource, fragmentSource);
    }

    Ref<VertexBuffer>
    OpenGLRenderer::CreateVertexBuffer(const void* data, size_t size, const std::span<VertexBuffer::Element>& layout) {
        return Ref<OpenGLVertexBuffer>::Create(this, data, size, layout);
    }

    Ref<IndexBuffer> OpenGLRenderer::CreateIndexBuffer(const std::span<uint32_t>& indices) {
        return Ref<OpenGLIndexBuffer>::Create(this, indices);
    }

    Ref<Texture> OpenGLRenderer::CreateTexture(const glm::u8vec4* pixels, size_t width, size_t height) {
        return Ref<OpenGLTexture>::Create(this, pixels, width, height);
    }

    Ref<Texture> OpenGLRenderer::CreateTexture(const glm::vec4* pixels, size_t width, size_t height) {
        return Ref<OpenGLTexture>::Create(this, pixels, width, height);
    }

    Ref<Texture> OpenGLRenderer::CreateTexture(size_t width, size_t height) {
        return Ref<OpenGLTexture>::Create(this, width, height);
    }

    Ref<Framebuffer> OpenGLRenderer::CreateFramebuffer(Ref<Texture> colorAttachment, bool hasDepthStencilAttachment) {
        return Ref<OpenGLFramebuffer>::Create(this, colorAttachment.As<OpenGLTexture>(), hasDepthStencilAttachment);
    }

    void OpenGLRenderer::Clear(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                GL_STENCIL_BUFFER_BIT); // TODO: Do we clear the depth and stencil buffer here?
    }

    void OpenGLRenderer::BeginScene(const glm::mat4& cameraTransform,
                                    const glm::mat4& projectionMatrix,
                                    bool depthTest,
                                    Ref<Framebuffer> framebuffer) {
        ViewMatrix         = glm::inverse(cameraTransform);
        ProjectionMatrix   = projectionMatrix;
        CurrentFramebuffer = framebuffer;
        if (CurrentFramebuffer) {
            CurrentFramebuffer.As<OpenGLFramebuffer>()->Bind();
            glViewport(0, 0, CurrentFramebuffer->GetWidth(), CurrentFramebuffer->GetHeight());
        } else {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            glViewport(0, 0, GetWindow()->GetWidth(), GetWindow()->GetHeight());
        }
        if (depthTest) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void OpenGLRenderer::EndScene() {
        if (CurrentFramebuffer) {
            CurrentFramebuffer.As<OpenGLFramebuffer>()->Unbind();
            CurrentFramebuffer = nullptr;
        }
    }

    void OpenGLRenderer::Draw(Ref<VertexBuffer> vertexBuffer,
                              Ref<Shader> shader,
                              size_t first,
                              size_t count,
                              const Transform& transform,
                              const Material& material) {
        vertexBuffer.As<OpenGLVertexBuffer>()->Bind();
        Ref<OpenGLShader> openGLShader = shader.As<OpenGLShader>();
        openGLShader->Bind();
        openGLShader->SetMat4Uniform("u_ModelMatrix", transform.ToMatrix());
        openGLShader->SetMat4Uniform("u_ViewMatrix", ViewMatrix);
        openGLShader->SetMat4Uniform("u_ProjectionMatrix", ProjectionMatrix);
        openGLShader->SetVec4Uniform("u_Color", material.Color);
        if (CurrentFramebuffer) {
            openGLShader->SetVec2Uniform("u_ScreenSize", { CurrentFramebuffer->GetWidth(), CurrentFramebuffer->GetHeight() });
        } else {
            openGLShader->SetVec2Uniform("u_ScreenSize", { GetWindow()->GetWidth(), GetWindow()->GetHeight() });
        }
        Ref<Texture> texture = material.Texture;
        if (!texture) {
            if (!WhitePixelTexture) {
                glm::u8vec4 pixels = { 0xFF, 0xFF, 0xFF, 0xFF };
                WhitePixelTexture  = CreateTexture(&pixels, 1, 1);
            }
            texture = WhitePixelTexture;
        }
        texture.As<OpenGLTexture>()->Bind(0);
        openGLShader->SetIntUniform("u_Texture", 0);
        glDrawArrays(GL_TRIANGLES, static_cast<int32_t>(first), static_cast<uint32_t>(count));
    }

    void OpenGLRenderer::DrawIndexed(Ref<VertexBuffer> vertexBuffer,
                                     Ref<IndexBuffer> indexBuffer,
                                     Ref<Shader> shader,
                                     const Transform& transform,
                                     const Material& material) {
        vertexBuffer.As<OpenGLVertexBuffer>()->Bind();
        indexBuffer.As<OpenGLIndexBuffer>()->Bind();
        Ref<OpenGLShader> openGLShader = shader.As<OpenGLShader>();
        openGLShader->Bind();
        openGLShader->SetMat4Uniform("u_ModelMatrix", transform.ToMatrix());
        openGLShader->SetMat4Uniform("u_ViewMatrix", ViewMatrix);
        openGLShader->SetMat4Uniform("u_ProjectionMatrix", ProjectionMatrix);
        openGLShader->SetVec4Uniform("u_Color", material.Color);
        if (CurrentFramebuffer) {
            openGLShader->SetVec2Uniform("u_ScreenSize", { CurrentFramebuffer->GetWidth(), CurrentFramebuffer->GetHeight() });
        } else {
            openGLShader->SetVec2Uniform("u_ScreenSize", { GetWindow()->GetWidth(), GetWindow()->GetHeight() });
        }
        Ref<Texture> texture = material.Texture;
        if (!texture) {
            if (!WhitePixelTexture) {
                glm::u8vec4 pixels = { 0xFF, 0xFF, 0xFF, 0xFF };
                WhitePixelTexture  = CreateTexture(&pixels, 1, 1);
            }
            texture = WhitePixelTexture;
        }
        texture.As<OpenGLTexture>()->Bind(0);
        openGLShader->SetIntUniform("u_Texture", 0);
        glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(indexBuffer->GetIndexCount()), GL_UNSIGNED_INT, nullptr);
    }

}
