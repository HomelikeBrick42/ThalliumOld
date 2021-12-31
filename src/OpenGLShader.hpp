#pragma once

#include "Base.hpp"
#include "Shader.hpp"
#include "OpenGLRenderer.hpp"

class OpenGLShader final: public Shader {
    friend class OpenGLRenderer;
public:
    OpenGLShader(Ref<OpenGLRenderer> renderer, const std::string& vertexSource, const std::string& fragmentSource);
    ~OpenGLShader() final;
public:
    void Bind();
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
