#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Renderer.hpp"

#include <glm/mat4x4.hpp>

namespace Thallium {

    constexpr uint32_t GL_DEPTH_TEST = 2929;

    constexpr uint32_t GL_COLOR_BUFFER_BIT   = 16384;
    constexpr uint32_t GL_DEPTH_BUFFER_BIT   = 256;
    constexpr uint32_t GL_STENCIL_BUFFER_BIT = 1024;

    constexpr uint32_t GL_FLOAT         = 5126;
    constexpr uint32_t GL_UNSIGNED_INT  = 5125;
    constexpr uint32_t GL_UNSIGNED_BYTE = 5121;

    constexpr uint32_t GL_RGBA32F          = 34836;
    constexpr uint32_t GL_DEPTH24_STENCIL8 = 35056;

    constexpr uint32_t GL_RGBA = 6408;

    constexpr uint32_t GL_ARRAY_BUFFER         = 34962;
    constexpr uint32_t GL_ELEMENT_ARRAY_BUFFER = 34963;
    constexpr uint32_t GL_STATIC_DRAW          = 35044;

    constexpr uint32_t GL_VERTEX_SHADER   = 35633;
    constexpr uint32_t GL_FRAGMENT_SHADER = 35632;

    constexpr uint32_t GL_COMPILE_STATUS  = 35713;
    constexpr uint32_t GL_LINK_STATUS     = 35714;
    constexpr uint32_t GL_INFO_LOG_LENGTH = 35716;

    constexpr uint32_t GL_TEXTURE_2D = 3553;
    constexpr uint32_t GL_TEXTURE0   = 33984;

    constexpr uint32_t GL_TEXTURE_MIN_FILTER = 10241;
    constexpr uint32_t GL_TEXTURE_MAG_FILTER = 10240;
    constexpr uint32_t GL_LINEAR             = 9729;
    constexpr uint32_t GL_NEAREST            = 9728;

    constexpr uint32_t GL_TEXTURE_WRAP_S = 10242;
    constexpr uint32_t GL_TEXTURE_WRAP_T = 10243;
    constexpr uint32_t GL_CLAMP_TO_EDGE  = 33071;

    constexpr uint32_t GL_RENDERBUFFER = 36161;

    constexpr uint32_t GL_FRAMEBUFFER      = 36160;
    constexpr uint32_t GL_READ_FRAMEBUFFER = 36008;
    constexpr uint32_t GL_DRAW_FRAMEBUFFER = 36009;

    constexpr uint32_t GL_COLOR_ATTACHMENT0        = 36064;
    constexpr uint32_t GL_DEPTH_STENCIL_ATTACHMENT = 33306;

    constexpr uint32_t GL_FRAMEBUFFER_COMPLETE = 36053;

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
    OPENGL_FUNCTION(void, glUniform1i, TYPE(int32_t) location, TYPE(int32_t) v0)                                                \
                                                                                                                                \
    OPENGL_FUNCTION(void, glGenTextures, TYPE(uint32_t) n, TYPE(uint32_t*) textures)                                            \
    OPENGL_FUNCTION(void, glDeleteTextures, TYPE(uint32_t) n, TYPE(uint32_t*) textures)                                         \
    OPENGL_FUNCTION(void, glActiveTexture, TYPE(uint32_t) texture)                                                              \
    OPENGL_FUNCTION(void, glBindTexture, TYPE(uint32_t) target, TYPE(uint32_t) texture)                                         \
    OPENGL_FUNCTION(void, glTexParameteri, TYPE(uint32_t) target, TYPE(uint32_t) pname, TYPE(int32_t) param)                    \
    OPENGL_FUNCTION(void,                                                                                                       \
                    glTexImage2D,                                                                                               \
                    TYPE(uint32_t) target,                                                                                      \
                    TYPE(int32_t) level,                                                                                        \
                    TYPE(int32_t) internalFormat,                                                                               \
                    TYPE(uint32_t) width,                                                                                       \
                    TYPE(uint32_t) height,                                                                                      \
                    TYPE(int32_t) border,                                                                                       \
                    TYPE(uint32_t) format,                                                                                      \
                    TYPE(uint32_t) type,                                                                                        \
                    TYPE(const void*) data)                                                                                     \
    OPENGL_FUNCTION(void,                                                                                                       \
                    glGetTexImage,                                                                                              \
                    TYPE(uint32_t) target,                                                                                      \
                    TYPE(int32_t) level,                                                                                        \
                    TYPE(uint32_t) format,                                                                                      \
                    TYPE(uint32_t) type,                                                                                        \
                    TYPE(void*) img)                                                                                            \
                                                                                                                                \
    OPENGL_FUNCTION(void, glGenFramebuffers, TYPE(uint32_t) n, TYPE(uint32_t*) ids)                                             \
    OPENGL_FUNCTION(void, glDeleteFramebuffers, TYPE(uint32_t) n, TYPE(uint32_t*) ids)                                          \
    OPENGL_FUNCTION(void, glBindFramebuffer, TYPE(uint32_t) target, TYPE(uint32_t) framebuffer)                                 \
    OPENGL_FUNCTION(uint32_t, glCheckFramebufferStatus, TYPE(uint32_t) target)                                                  \
    OPENGL_FUNCTION(void,                                                                                                       \
                    glBlitFramebuffer,                                                                                          \
                    TYPE(int32_t) srcX0,                                                                                        \
                    TYPE(int32_t) srcY0,                                                                                        \
                    TYPE(int32_t) srcX1,                                                                                        \
                    TYPE(int32_t) srcY1,                                                                                        \
                    TYPE(int32_t) dstX0,                                                                                        \
                    TYPE(int32_t) dstY0,                                                                                        \
                    TYPE(int32_t) dstX1,                                                                                        \
                    TYPE(int32_t) dstY1,                                                                                        \
                    TYPE(uint32_t) mask,                                                                                        \
                    TYPE(uint32_t) filter)                                                                                      \
    OPENGL_FUNCTION(void,                                                                                                       \
                    glFramebufferTexture2D,                                                                                     \
                    TYPE(uint32_t) target,                                                                                      \
                    TYPE(uint32_t) attachment,                                                                                  \
                    TYPE(uint32_t) textarget,                                                                                   \
                    TYPE(uint32_t) texture,                                                                                     \
                    TYPE(int32_t) level)                                                                                        \
    OPENGL_FUNCTION(void,                                                                                                       \
                    glFramebufferRenderbuffer,                                                                                  \
                    TYPE(uint32_t) target,                                                                                      \
                    TYPE(uint32_t) attachment,                                                                                  \
                    TYPE(uint32_t) renderbuffertarget,                                                                          \
                    TYPE(uint32_t) renderbuffer)                                                                                \
                                                                                                                                \
    OPENGL_FUNCTION(void, glGenRenderbuffers, TYPE(uint32_t) n, TYPE(uint32_t*) renderbuffers)                                  \
    OPENGL_FUNCTION(void, glDeleteRenderbuffers, TYPE(uint32_t) n, TYPE(uint32_t*) renderbuffers)                               \
    OPENGL_FUNCTION(void, glBindRenderbuffer, TYPE(uint32_t) target, TYPE(uint32_t) renderbuffer)                               \
    OPENGL_FUNCTION(void,                                                                                                       \
                    glRenderbufferStorage,                                                                                      \
                    TYPE(uint32_t) target,                                                                                      \
                    TYPE(uint32_t) internalFormat,                                                                              \
                    TYPE(uint32_t) width,                                                                                       \
                    TYPE(uint32_t) height)                                                                                      \
                                                                                                                                \
    OPENGL_FUNCTION(void, glDrawArrays, TYPE(uint32_t) mode, TYPE(int32_t) first, TYPE(uint32_t) count)                         \
    OPENGL_FUNCTION(                                                                                                            \
        void, glDrawElements, TYPE(uint32_t) mode, TYPE(uint32_t) count, TYPE(uint32_t) type, TYPE(const void*) indices)

    class OpenGLRenderer: public Renderer {
    public:
#define TYPE(type)                      type
#define OPENGL_FUNCTION(ret, name, ...) typedef ret _cdecl name##FunctionType(__VA_ARGS__);
        OPENGL_FUNCTIONS
#undef OPENGL_FUNCTION
#undef TYPE
    protected:
        OpenGLRenderer() = default;
    public:
    public:
        virtual ~OpenGLRenderer() = default;
    public:
        void Clear(const glm::vec4& color) final;
        void BeginScene(const Transform& cameraTransform,
                        const glm::mat4& projectionMatrix,
                        bool depthTest,
                        Ref<Framebuffer> framebuffer = nullptr) final;
        void EndScene() final;
        void Draw(Ref<VertexBuffer> vertexBuffer,
                  Ref<Shader> shader,
                  size_t first,
                  size_t count,
                  const Transform& transform,
                  const Material& material) final;
        void DrawIndexed(Ref<VertexBuffer> vertexBuffer,
                         Ref<IndexBuffer> indexBuffer,
                         Ref<Shader> shader,
                         const Transform& transform,
                         const Material& material) final;
        Ref<VertexBuffer> CreateVertexBuffer(const void* data, size_t size, const std::span<VertexBuffer::Element>& layout) final;
        Ref<IndexBuffer> CreateIndexBuffer(const std::span<uint32_t>& indices) final;
        Ref<Texture> CreateTexture(const glm::u8vec4* pixels, size_t width, size_t height) final;
        Ref<Texture> CreateTexture(const glm::vec4* pixels, size_t width, size_t height) final;
        Ref<Texture> CreateTexture(size_t width, size_t height) final;
        Ref<Framebuffer> CreateFramebuffer(Ref<Texture> colorAttachment, bool hasDepthStencilAttachment) final;
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
        Ref<Texture> WhitePixelTexture;
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;
        Ref<Framebuffer> CurrentFramebuffer;
    };

#if !defined(KEEP_OPENGL_FUNCTIONS)
    #undef OPENGL_FUNCTIONS
#endif

}
