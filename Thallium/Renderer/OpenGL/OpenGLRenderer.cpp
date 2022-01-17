#include "Thallium/Renderer/OpenGL/OpenGLRenderer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLShader.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLVertexBuffer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLIndexBuffer.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLTexture.hpp"

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

    void OpenGLRenderer::Clear(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // TODO: Do we clear the depth buffer here?
    }

    void OpenGLRenderer::BeginScene(const Transform& cameraTransform, const glm::mat4& projectionMatrix, bool depthTest) {
        ViewMatrix       = glm::inverse(cameraTransform.ToMatrix());
        ProjectionMatrix = projectionMatrix;
        if (depthTest) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void OpenGLRenderer::EndScene() {}

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
        glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(indexBuffer->GetIndexCount()), GL_UNSIGNED_INT, nullptr);
    }

}
