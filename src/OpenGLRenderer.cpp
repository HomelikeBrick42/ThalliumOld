#include "OpenGLRenderer.hpp"
#include "OpenGLShader.hpp"
#include "OpenGLVertexBuffer.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <format>
#include <ranges>

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
        // TODO: Ignore whitespace
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

void OpenGLRenderer::Draw(Ref<VertexBuffer> vertexBuffer, Ref<Shader> shader, size_t first, size_t count) {
    vertexBuffer.As<OpenGLVertexBuffer>()->Bind();
    shader.As<OpenGLShader>()->Bind();
    glDrawArrays(GL_TRIANGLES, static_cast<int32_t>(first), static_cast<uint32_t>(count));
}
