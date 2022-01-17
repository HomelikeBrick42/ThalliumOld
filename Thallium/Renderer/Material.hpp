#pragma once

#include "Thallium/Core/Base.hpp"
#include "Thallium/Renderer/Texture.hpp"

#include <glm/vec4.hpp>

namespace Thallium {

    struct Material {
        glm::vec4 Color      = { 1.0f, 1.0f, 1.0f, 1.0f };
        Ref<Texture> Texture = nullptr; // nullptr is a single white pixel image
    };

}
