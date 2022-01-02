#include "ObjLoader.hpp"

#include <fstream>
#include <iostream>
#include <format>
#include <sstream>
#include <optional>

namespace Obj {

    std::vector<Object> Load(const std::string& filepath) {
        std::vector<Object> objects;

        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << std::format("Unable to open obj file '{}'", filepath) << std::endl;
            std::exit(1);
        }

        std::optional<Object*> currentObject;
        std::string line;
        while (std::getline(file, line)) {
            if (line.length() == 0) {
                break;
            }
            std::stringstream stream{ line };
            char command = {};
            stream >> command;
            switch (command) {
                case '#': {
                } break;

                case 'o': {
                    currentObject = &objects.emplace_back();
                    stream >> (**currentObject).Name;
                } break;

                case 'v': {
                    char subCommand = stream.peek();
                    if (subCommand == 't') {
                        stream >> subCommand;
                        glm::vec2 uv = {};
                        stream >> uv.x >> uv.y;
                        (**currentObject).UVs.push_back(uv);
                    } else if (subCommand == 'n') {
                        stream >> subCommand;
                        glm::vec3 normal = {};
                        stream >> normal.x >> normal.y >> normal.z;
                        (**currentObject).Normals.push_back(normal);
                    } else {
                        if (subCommand != ' ')
                            goto Default;
                        glm::vec3 vertexPos = {};
                        stream >> vertexPos.x >> vertexPos.y >> vertexPos.z;
                        (**currentObject).Positions.push_back(vertexPos);
                    }
                } break;

                case 's': {
                    // TODO: Unimplemented
                } break;

                case 'f': {
                    glm::uvec3 positions = {};
                    glm::uvec3 uvs       = {};
                    glm::uvec3 normals   = {};
                    char slash           = {};

                    stream >> positions[0];
                    stream >> slash;
                    if (slash != '/')
                        goto Default;
                    stream >> uvs[0];
                    stream >> slash;
                    if (slash != '/')
                        goto Default;
                    stream >> normals[0];

                    stream >> positions[1];
                    stream >> slash;
                    if (slash != '/')
                        goto Default;
                    stream >> uvs[1];
                    stream >> slash;
                    if (slash != '/')
                        goto Default;
                    stream >> normals[1];

                    stream >> positions[2];
                    stream >> slash;
                    if (slash != '/')
                        goto Default;
                    stream >> uvs[2];
                    stream >> slash;
                    if (slash != '/')
                        goto Default;
                    stream >> normals[2];

                    positions -= 1;
                    normals -= 1;
                    uvs -= 1;
                    (**currentObject).Faces.push_back(Face{ .Positions = positions, .Normals = normals, .UVs = uvs });
                } break;

                default:
Default: {
    std::cerr << std::format("Unknown command in obj file '{}'", line) << std::endl;
    std::exit(1);
} break;
            }
        }

        return objects;
    }
}
