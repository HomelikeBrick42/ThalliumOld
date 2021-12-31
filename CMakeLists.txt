cmake_minimum_required(VERSION 3.20)
project(Physics)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        Physics
        src/Base.hpp
        src/Main.cpp
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
target_link_libraries(Physics OpenGL32)
