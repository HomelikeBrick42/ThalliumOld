#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
    glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
    glm::quat Rotation = glm::identity<glm::quat>();
    glm::vec3 Scale    = { 1.0f, 1.0f, 1.0f };

    glm::mat4 ToMatrix() {
        return glm::translate(glm::mat4(1.0f), Position) * glm::mat4_cast(Rotation) * glm::scale(glm::mat4(1.0f), Scale);
    }

    operator glm::mat4() {
        return ToMatrix();
    }
};
