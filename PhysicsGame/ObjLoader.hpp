#pragma once

#include <string>
#include <vector>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace Obj {
    struct Face {
        glm::uvec3 Positions;
        glm::uvec3 Normals;
        glm::uvec3 UVs;
    };

    struct Object {
        std::string Name;
        std::vector<glm::vec3> Positions;
        std::vector<glm::vec3> Normals;
        std::vector<glm::vec2> UVs;
        std::vector<Face> Faces;
    };

    std::vector<Object> Load(const std::string& filepath);
}
