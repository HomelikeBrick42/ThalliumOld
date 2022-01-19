#include "Camera.hpp"

Camera::Camera(float speed, float lookSpeed, float fov, size_t width, size_t height)
    : Transform()
    , ProjectionMatrix(glm::identity<glm::mat4>())
    , Speed(speed)
    , LookSpeed(lookSpeed)
    , FOV(fov)
    , Width(width)
    , Height(height)
    , WPressed(false)
    , SPressed(false)
    , APressed(false)
    , DPressed(false)
    , QPressed(false)
    , EPressed(false)
    , SpacePressed(false)
    , ShiftPressed(false)
    , MouseX(0)
    , MouseY(0) {
    UpdateProjectionMatrix();
}

void Camera::OnUpdate(float dt) {
    float deltaX = (float)MouseX / (float)Width * LookSpeed;
    float deltaY = (float)MouseY / (float)Height * LookSpeed;

    Transform.Rotation = glm::rotate(glm::identity<glm::quat>(), deltaX, Transform.GetUp()) * Transform.Rotation;
    Transform.Rotation = glm::rotate(glm::identity<glm::quat>(), deltaY, Transform.GetRight()) * Transform.Rotation;

    float rotationSpeed = 90.0f * LookSpeed;
    float rotation      = ((QPressed ? rotationSpeed : 0) + (EPressed ? -rotationSpeed : 0)) * dt;
    Transform.Rotation =
        glm::rotate(glm::identity<glm::quat>(), glm::radians(rotation), Transform.GetForward()) * Transform.Rotation;

    if (WPressed)
        Transform.Position += Transform.GetForward() * Speed * dt;
    if (SPressed)
        Transform.Position -= Transform.GetForward() * Speed * dt;
    if (APressed)
        Transform.Position -= Transform.GetRight() * Speed * dt;
    if (DPressed)
        Transform.Position += Transform.GetRight() * Speed * dt;
    if (SpacePressed)
        Transform.Position += Transform.GetUp() * Speed * dt;
    if (ShiftPressed)
        Transform.Position -= Transform.GetUp() * Speed * dt;

    MouseX = 0;
    MouseY = 0;
}

void Camera::OnKey(Thallium::KeyCode key, bool pressed) {
    switch (key) {
        case Thallium::KeyCode_W: {
            WPressed = pressed;
        } break;

        case Thallium::KeyCode_S: {
            SPressed = pressed;
        } break;

        case Thallium::KeyCode_A: {
            APressed = pressed;
        } break;

        case Thallium::KeyCode_D: {
            DPressed = pressed;
        } break;

        case Thallium::KeyCode_Q: {
            QPressed = pressed;
        } break;

        case Thallium::KeyCode_E: {
            EPressed = pressed;
        } break;

        case Thallium::KeyCode_Space: {
            SpacePressed = pressed;
        } break;

        case Thallium::KeyCode_Shift: {
            ShiftPressed = pressed;
        } break;

        default: {
        } break;
    }
}

void Camera::OnMouseMove(int32_t x, int32_t y) {
    MouseX += x;
    MouseY += y;
}

void Camera::UpdateProjectionMatrix() {
    ProjectionMatrix = glm::perspective(glm::radians(FOV), (float)Width / (float)Height, 0.001f, 1000.0f);
}
