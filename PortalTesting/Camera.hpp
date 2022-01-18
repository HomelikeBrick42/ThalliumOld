#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Core/Transform.hpp"
#include "Thallium/Core/Keycode.hpp"

struct Camera {
public:
    Camera(float speed, float lookSpeed, float fov, size_t width, size_t height);
public:
    void OnUpdate(float dt);
    void OnKey(Thallium::KeyCode key, bool pressed);
    void OnMouseMove(int32_t x, int32_t y);
    void UpdateProjectionMatrix();
public:
    Thallium::Transform Transform;
    glm::mat4 ProjectionMatrix;
    float Speed;
    float LookSpeed;
    float FOV;
    size_t Width;
    size_t Height;
private:
    bool WPressed;
    bool APressed;
    bool SPressed;
    bool DPressed;
    bool QPressed;
    bool EPressed;
    bool SpacePressed;
    bool ShiftPressed;
    int32_t MouseX;
    int32_t MouseY;
};
