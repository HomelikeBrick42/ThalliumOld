#shader shared

#version 440 core

uniform mat4 u_ModelMatrix = mat4(1.0);
uniform mat4 u_ViewMatrix = mat4(1.0);
uniform mat4 u_ProjectionMatrix = mat4(1.0);

uniform vec4 u_Color = vec4(1.0f);
uniform sampler2D u_Texture;

#shader vertex

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_Coord;

layout(location = 0) out vec2 v_Coord;

void main() {
    v_Coord = a_Coord;
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * a_Position;
}

#shader fragment

layout(location = 0) out vec4 o_Color;

layout(location = 0) in vec2 v_Coord;

void main() {
    if (dot(v_Coord, v_Coord) > 1.0) {
        discard;
    }
    o_Color = u_Color;
}
