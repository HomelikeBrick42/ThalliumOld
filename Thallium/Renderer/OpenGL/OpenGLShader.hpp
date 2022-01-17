#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Shader.hpp"
#include "Thallium/Renderer/OpenGL/OpenGLRenderer.hpp"

#include <glm/fwd.hpp>

namespace Thallium {

    class OpenGLShader final: public Shader {
        friend class OpenGLRenderer;
    public:
        OpenGLShader(Ref<OpenGLRenderer> renderer, const std::string& vertexSource, const std::string& fragmentSource);
        ~OpenGLShader() final;
    public:
        void Bind();
        void SetMat4Uniform(const std::string& name, const glm::mat4& matrix);
        void SetVec4Uniform(const std::string& name, const glm::vec4& value);
        void SetIntUniform(const std::string& name, int32_t value);
    public:
        Ref<Renderer> GetRenderer() final {
            return Renderer;
        }
    private:
        uint32_t CreateShader(uint32_t type, const char* source);
    private:
        Ref<OpenGLRenderer> Renderer;
        uint32_t Program;
    };

}
