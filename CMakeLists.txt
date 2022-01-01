cmake_minimum_required(VERSION 3.20)
project(Thallium)
project(Thallium)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vendor/glm)

add_library(
        Thallium
        Thallium/Base.hpp
        Thallium/IndexBuffer.hpp
        Thallium/OpenGLIndexBuffer.cpp
        Thallium/OpenGLIndexBuffer.hpp
        Thallium/OpenGLRenderer.cpp
        Thallium/OpenGLRenderer.hpp
        Thallium/OpenGLShader.cpp
        Thallium/OpenGLShader.hpp
        Thallium/OpenGLVertexBuffer.cpp
        Thallium/OpenGLVertexBuffer.hpp
        Thallium/Ref.hpp
        Thallium/Renderer.hpp
        Thallium/Shader.hpp
        Thallium/VertexBuffer.hpp
        Thallium/Window.hpp
        Thallium/WindowsOpenGLRenderer.cpp
        Thallium/WindowsOpenGLRenderer.hpp
        Thallium/WindowsWindow.cpp
        Thallium/WindowsWindow.hpp)
target_link_libraries(Thallium glm)
target_compile_options(Thallium PRIVATE -DGLM_FORCE_SWIZZLE -DGLM_FORCE_LEFT_HANDED)
target_include_directories(Thallium PRIVATE Thallium vendor/glm)
target_link_libraries(Thallium OpenGL32)

add_executable(
        PhysicsGame
        PhysicsGame/Main.cpp)
target_link_libraries(PhysicsGame Thallium glm)
target_compile_options(PhysicsGame PRIVATE -DGLM_FORCE_SWIZZLE -DGLM_FORCE_LEFT_HANDED)
target_include_directories(PhysicsGame PRIVATE Thallium vendor/glm)
