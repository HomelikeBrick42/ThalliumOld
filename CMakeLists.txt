cmake_minimum_required(VERSION 3.20)
project(Thallium)
project(Thallium)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vendor/glm)

add_library(
        Thallium
        Thallium/Core/Base.hpp
        Thallium/Core/Clock.cpp
        Thallium/Core/Clock.hpp
        Thallium/Core/Keycode.hpp
        Thallium/Core/Ref.hpp
        Thallium/Core/Transform.hpp
        Thallium/Core/Window.hpp
        Thallium/Core/Windows/WindowsWindow.cpp
        Thallium/Core/Windows/WindowsWindow.hpp
        Thallium/Renderer/Framebuffer.hpp
        Thallium/Renderer/IndexBuffer.hpp
        Thallium/Renderer/Material.hpp
        Thallium/Renderer/Renderer.hpp
        Thallium/Renderer/Shader.hpp
        Thallium/Renderer/Texture.hpp
        Thallium/Renderer/VertexBuffer.hpp
        Thallium/Renderer/OpenGL/OpenGLFramebuffer.cpp
        Thallium/Renderer/OpenGL/OpenGLFramebuffer.hpp
        Thallium/Renderer/OpenGL/OpenGLIndexBuffer.cpp
        Thallium/Renderer/OpenGL/OpenGLIndexBuffer.hpp
        Thallium/Renderer/OpenGL/OpenGLRenderer.cpp
        Thallium/Renderer/OpenGL/OpenGLRenderer.hpp
        Thallium/Renderer/OpenGL/OpenGLShader.cpp
        Thallium/Renderer/OpenGL/OpenGLShader.hpp
        Thallium/Renderer/OpenGL/OpenGLTexture.cpp
        Thallium/Renderer/OpenGL/OpenGLTexture.hpp
        Thallium/Renderer/OpenGL/OpenGLVertexBuffer.cpp
        Thallium/Renderer/OpenGL/OpenGLVertexBuffer.hpp
        Thallium/Renderer/OpenGL/Windows/WindowsOpenGLRenderer.cpp
        Thallium/Renderer/OpenGL/Windows/WindowsOpenGLRenderer.hpp)
target_link_libraries(Thallium glm)
target_compile_options(Thallium PRIVATE -DGLM_FORCE_SWIZZLE -DGLM_FORCE_LEFT_HANDED -D_USE_MATH_DEFINES)
target_include_directories(Thallium PRIVATE ./ vendor/glm)
target_link_libraries(Thallium OpenGL32)

add_executable(
        PhysicsGame
        PhysicsGame/Main.cpp
        PhysicsGame/ObjLoader.cpp
        PhysicsGame/ObjLoader.hpp)
target_link_libraries(PhysicsGame Thallium glm)
target_compile_options(PhysicsGame PRIVATE -DGLM_FORCE_SWIZZLE -DGLM_FORCE_LEFT_HANDED -D_USE_MATH_DEFINES)
target_include_directories(PhysicsGame PRIVATE ./ ./PhysicsGame vendor/glm)

add_executable(
        Sandbox
        Sandbox/Camera.cpp
        Sandbox/Camera.hpp
        Sandbox/Main.cpp)
target_link_libraries(Sandbox Thallium glm)
target_compile_options(Sandbox PRIVATE -DGLM_FORCE_SWIZZLE -DGLM_FORCE_LEFT_HANDED -D_USE_MATH_DEFINES)
target_include_directories(Sandbox PRIVATE ./ ./Sandbox vendor/glm)
