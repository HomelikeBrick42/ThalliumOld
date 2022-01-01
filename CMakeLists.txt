cmake_minimum_required(VERSION 3.20)
project(Thallium)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(glm)
add_executable(
        Thallium
        src/Base.hpp
        src/IndexBuffer.hpp
        src/Main.cpp
        src/OpenGLIndexBuffer.cpp
        src/OpenGLIndexBuffer.hpp
        src/OpenGLRenderer.cpp
        src/OpenGLRenderer.hpp
        src/OpenGLShader.cpp
        src/OpenGLShader.hpp
        src/OpenGLVertexBuffer.cpp
        src/OpenGLVertexBuffer.hpp
        src/Ref.hpp
        src/Renderer.hpp
        src/Shader.hpp
        src/VertexBuffer.hpp
        src/Window.hpp
        src/WindowsOpenGLRenderer.cpp
        src/WindowsOpenGLRenderer.hpp
        src/WindowsWindow.cpp
        src/WindowsWindow.hpp)
target_compile_options(Thallium PRIVATE -DGLM_FORCE_SWIZZLE -DGLM_FORCE_LEFT_HANDED)
target_include_directories(Thallium PRIVATE glm/)
target_link_libraries(Thallium OpenGL32)
