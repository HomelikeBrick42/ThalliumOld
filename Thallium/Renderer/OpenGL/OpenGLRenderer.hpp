#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Renderer.hpp"

#include <glm/mat4x4.hpp>

namespace Thallium {

    constexpr uint32_t GL_DEPTH_TEST = 2929;

    constexpr uint32_t GL_COLOR_BUFFER_BIT = 16384;
    constexpr uint32_t GL_DEPTH_BUFFER_BIT = 256;

    constexpr uint32_t GL_FLOAT        = 5126;
    constexpr uint32_t GL_UNSIGNED_INT = 5125;

    constexpr uint32_t GL_ARRAY_BUFFER         = 34962;
    constexpr uint32_t GL_ELEMENT_ARRAY_BUFFER = 34963;
    constexpr uint32_t GL_STATIC_DRAW          = 35044;

    constexpr uint32_t GL_VERTEX_SHADER   = 35633;
    constexpr uint32_t GL_FRAGMENT_SHADER = 35632;

    constexpr uint32_t GL_COMPILE_STATUS  = 35713;
    constexpr uint32_t GL_LINK_STATUS     = 35714;
    constexpr uint32_t GL_INFO_LOG_LENGTH = 35716;

    constexpr uint32_t GL_TRIANGLES = 4;

#define OPENGL_FUNCTIONS                                                                                                        \
    OPENGL_FUNCTION(void, glEnable, TYPE(uint32_t) cap)                                                                         \
    OPENGL_FUNCTION(void, glDisable, TYPE(uint32_t) cap)                                                                        \
                                                                                                                                \
    OPENGL_FUNCTION(void, glClearColor, TYPE(float) r, TYPE(float) g, TYPE(float) b, TYPE(float) a)                             \
    OPENGL_FUNCTION(void, glClear, TYPE(uint32_t) mask)                                                                         \
                                                                                                                                \
    OPENGL_FUNCTION(void, glViewport, TYPE(int32_t) x, TYPE(int32_t) y, TYPE(uint32_t) width, TYPE(uint32_t) height)            \
                                                                                                                                \
    OPENGL_FUNCTION(void, glGenVertexArrays, TYPE(uint32_t) n, TYPE(uint32_t*) arrays)                                          \
    OPENGL_FUNCTION(void, glDeleteVertexArrays, TYPE(uint32_t) n, TYPE(uint32_t*) arrays)                                       \
    OPENGL_FUNCTION(void, glBindVertexArray, TYPE(uint32_t) array)                                                              \
    OPENGL_FUNCTION(void, glEnableVertexAttribArray, TYPE(uint32_t) index)                                                      \
    OPENGL_FUNCTION(void,                                                                                                       \
                    glVertexAttribPointer,                                                                                      \
                    TYPE(uint32_t) index,                                                                                       \
                    TYPE(int32_t) size,                                                                                         \
                    TYPE(uint32_t) type,                                                                                        \
                    TYPE(bool) normalized,                                                                                      \
                    TYPE(uint32_t) stride,                                                                                      \
                    TYPE(const void*) pointer)                                                                                  \
                                                                                                                                \
    OPENGL_FUNCTION(void, glGenBuffers, TYPE(uint32_t) n, TYPE(uint32_t*) buffers)                                              \
    OPENGL_FUNCTION(void, glDeleteBuffers, TYPE(uint32_t) n, TYPE(uint32_t*) buffers)                                           \
    OPENGL_FUNCTION(void, glBindBuffer, TYPE(uint32_t) target, TYPE(uint32_t) buffer)                                           \
    OPENGL_FUNCTION(void, glBufferData, TYPE(uint32_t) target, TYPE(size_t) size, TYPE(const void*) data, TYPE(uint32_t) usage) \
                                                                                                                                \
    OPENGL_FUNCTION(uint32_t, glCreateShader, TYPE(uint32_t) shaderType)                                                        \
    OPENGL_FUNCTION(void, glDeleteShader, TYPE(uint32_t) shader)                                                                \
    OPENGL_FUNCTION(void,                                                                                                       \
                    glShaderSource,                                                                                             \
                    TYPE(uint32_t) shader,                                                                                      \
                    TYPE(uint32_t) count,                                                                                       \
                    TYPE(const char**) string,                                                                                  \
                    TYPE(const int32_t*) length)                                                                                \
    OPENGL_FUNCTION(void, glCompileShader, TYPE(uint32_t) shader)                                                               \
    OPENGL_FUNCTION(void, glGetShaderiv, TYPE(uint32_t) shader, TYPE(uint32_t) pname, TYPE(int32_t*) params)                    \
    OPENGL_FUNCTION(                                                                                                            \
        void, glGetShaderInfoLog, TYPE(uint32_t) shader, TYPE(uint32_t) maxLength, TYPE(uint32_t*) length, TYPE(char*) infoLog) \
                                                                                                                                \
    OPENGL_FUNCTION(uint32_t, glCreateProgram, TYPE(void))                                                                      \
    OPENGL_FUNCTION(void, glDeleteProgram, TYPE(uint32_t) program)                                                              \
    OPENGL_FUNCTION(void, glAttachShader, TYPE(uint32_t) program, TYPE(uint32_t) shader)                                        \
    OPENGL_FUNCTION(void, glDetachShader, TYPE(uint32_t) program, TYPE(uint32_t) shader)                                        \
    OPENGL_FUNCTION(void, glLinkProgram, TYPE(uint32_t) program)                                                                \
    OPENGL_FUNCTION(void, glGetProgramiv, TYPE(uint32_t) program, TYPE(uint32_t) pname, TYPE(int32_t*) params)                  \
    OPENGL_FUNCTION(void,                                                                                                       \
                    glGetProgramInfoLog,                                                                                        \
                    TYPE(uint32_t) program,                                                                                     \
                    TYPE(uint32_t) maxLength,                                                                                   \
                    TYPE(uint32_t*) length,                                                                                     \
                    TYPE(char*) infoLog)                                                                                        \
    OPENGL_FUNCTION(void, glUseProgram, TYPE(uint32_t) program)                                                                 \
                                                                                                                                \
    OPENGL_FUNCTION(int32_t, glGetUniformLocation, TYPE(uint32_t) program, TYPE(const char*) name)                              \
    OPENGL_FUNCTION(                                                                                                            \
        void, glUniformMatrix4fv, TYPE(int32_t) location, TYPE(uint32_t) count, TYPE(bool) transpose, TYPE(const float*) value) \
    OPENGL_FUNCTION(void, glUniform4fv, TYPE(int32_t) location, TYPE(uint32_t) count, TYPE(const float*) value)                 \
                                                                                                                                \
    OPENGL_FUNCTION(void, glDrawArrays, TYPE(uint32_t) mode, TYPE(int32_t) first, TYPE(uint32_t) count)                         \
    OPENGL_FUNCTION(                                                                                                            \
        void, glDrawElements, TYPE(uint32_t) mode, TYPE(uint32_t) count, TYPE(uint32_t) type, TYPE(const void*) indices)

#define TYPE(type)                      type
#define OPENGL_FUNCTION(ret, name, ...) typedef ret _cdecl name##FunctionType(__VA_ARGS__);
    OPENGL_FUNCTIONS
#undef OPENGL_FUNCTION
#undef TYPE

    class OpenGLRenderer: public Renderer {
    protected:
        OpenGLRenderer() = default;
    public:
    public:
        virtual ~OpenGLRenderer() = default;
    public:
        void Clear(const glm::vec4& color) final;
        void BeginScene(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, bool depthTest) final;
        void EndScene() final;
        void Draw(Ref<VertexBuffer> vertexBuffer,
                  Ref<Shader> shader,
                  size_t first,
                  size_t count,
                  const glm::mat4& transform,
                  const glm::vec4& color) final;
        void DrawIndexed(Ref<VertexBuffer> vertexBuffer,
                         Ref<IndexBuffer> indexBuffer,
                         Ref<Shader> shader,
                         const glm::mat4& transform,
                         const glm::vec4& color) final;
        Ref<VertexBuffer> CreateVertexBuffer(const void* data, size_t size, const std::span<VertexBuffer::Element>& layout) final;
        Ref<IndexBuffer> CreateIndexBuffer(const std::span<uint32_t>& indices) final;
    public:
        Ref<Shader> CreateShader(const std::string& filepath) final;
    public:
#define APPEND_TYPE(type) type
#define APPEND(x)         GLUE(APPEND_, x)
#define REMOVE_TYPE(type)
#define REMOVE(x) GLUE(REMOVE_, x)
#define OPENGL_FUNCTION(ret, name, ...)                     \
    ret name TRANSFORM(APPEND, (__VA_ARGS__)) {             \
        MakeContextCurrent();                               \
        return name##Func TRANSFORM(REMOVE, (__VA_ARGS__)); \
    }
        OPENGL_FUNCTIONS
#undef OPENGL_FUNCTION
#undef REMOVE
#undef REMOVE_TYPE
#undef APPEND
#undef APPEND_TYPE
    protected:
        virtual void MakeContextCurrent() = 0;
#define OPENGL_FUNCTION(ret, name, ...) name##FunctionType* name##Func;
        OPENGL_FUNCTIONS
#undef OPENGL_FUNCTION
    private:
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
    };

#if !defined(KEEP_OPENGL_FUNCTIONS)
    #undef OPENGL_FUNCTIONS
#endif

}
