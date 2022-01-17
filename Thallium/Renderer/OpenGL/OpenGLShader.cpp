#include "Thallium/Renderer/OpenGL/OpenGLShader.hpp"

#include <iostream>
#include <format>

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Thallium {

    OpenGLShader::OpenGLShader(Ref<OpenGLRenderer> renderer, const std::string& vertexSource, const std::string& fragmentSource)
        : Renderer(renderer), Program(UINT32_MAX) {
        uint32_t vertexShader = CreateShader(GL_VERTEX_SHADER, vertexSource.c_str());
        defer(Renderer->glDeleteShader(vertexShader));
        uint32_t fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());
        defer(Renderer->glDeleteShader(fragmentShader));

        Program = Renderer->glCreateProgram();
        Renderer->glAttachShader(Program, vertexShader);
        defer(Renderer->glDetachShader(Program, vertexShader));
        Renderer->glAttachShader(Program, fragmentShader);
        defer(Renderer->glDetachShader(Program, fragmentShader));

        Renderer->glLinkProgram(Program);

        int32_t isLinked = false;
        Renderer->glGetProgramiv(Program, GL_LINK_STATUS, &isLinked);
        if (!isLinked) {
            int32_t maxLength = 0;
            Renderer->glGetProgramiv(Program, GL_INFO_LOG_LENGTH, &maxLength);

            char* buffer = new char[maxLength];
            defer(delete[] buffer);
            Renderer->glGetShaderInfoLog(Program, maxLength, nullptr, buffer);

            std::cerr << std::format("Shader Linkage Failed: '{}'", buffer) << std::endl;
            std::exit(1);
        }
    }

    OpenGLShader::~OpenGLShader() {
        Renderer->glDeleteProgram(Program);
    }

    uint32_t OpenGLShader::CreateShader(uint32_t type, const char* source) {
        uint32_t shader = Renderer->glCreateShader(type);
        Renderer->glShaderSource(shader, 1, &source, nullptr);
        Renderer->glCompileShader(shader);

        int32_t isCompiled = false;
        Renderer->glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
        if (!isCompiled) {
            int32_t maxLength = 0;
            Renderer->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            char* buffer = new char[maxLength];
            defer(delete[] buffer);
            Renderer->glGetShaderInfoLog(shader, maxLength, nullptr, buffer);

            std::cerr << std::format("Shader Compilation Failed: '{}'", buffer) << std::endl;
            std::exit(1);
        }

        return shader;
    }

    void OpenGLShader::Bind() {
        Renderer->glUseProgram(Program);
    }

    void OpenGLShader::SetMat4Uniform(const std::string& name, const glm::mat4& matrix) {
        Bind();
        Renderer->glUniformMatrix4fv(Renderer->glGetUniformLocation(Program, name.c_str()), 1, false, glm::value_ptr(matrix));
    }

    void OpenGLShader::SetVec4Uniform(const std::string& name, const glm::vec4& value) {
        Bind();
        Renderer->glUniform4fv(Renderer->glGetUniformLocation(Program, name.c_str()), 1, glm::value_ptr(value));
    }

    void OpenGLShader::SetIntUniform(const std::string& name, int32_t value) {
        Bind();
        Renderer->glUniform1i(Renderer->glGetUniformLocation(Program, name.c_str()), value);
    }

}
