#pragma once

#include "Thallium/Core/Base.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Thallium {

    struct Transform {
        glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
        glm::quat Rotation = glm::identity<glm::quat>();
        glm::vec3 Scale    = { 1.0f, 1.0f, 1.0f };

        glm::mat4 ToMatrix() const {
            return glm::translate(glm::identity<glm::mat4>(), Position) * glm::toMat4(Rotation) *
                   glm::scale(glm::identity<glm::mat4>(), Scale);
        }

        operator glm::mat4() {
            return ToMatrix();
        }
    };

}
