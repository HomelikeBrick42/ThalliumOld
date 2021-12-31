#include "OpenGLShader.hpp"

#include <iostream>
#include <format>

OpenGLShader::OpenGLShader(Ref<OpenGLRenderer> renderer, const std::string& vertexSource, const std::string& fragmentSource)
    : Renderer(renderer), Program(0) {
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

        std::cerr << std::format("Shader Linkage Failed: '{s}'", buffer) << std::endl;
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

        std::cerr << std::format("Shader Compilation Failed: '{s}'", buffer) << std::endl;
        std::exit(1);
    }

    return shader;
}

void OpenGLShader::Bind() {
    Renderer->glUseProgram(Program);
}
