#shader shared

#version 440 core

layout(location = 0) uniform mat4 u_ModelMatrix = mat4(1.0);
layout(location = 1) uniform mat4 u_ViewMatrix = mat4(1.0);
layout(location = 2) uniform mat4 u_ProjectionMatrix = mat4(1.0);

#shader vertex

layout(location = 0) in vec4 a_Position;

void main() {
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * a_Position;
}

#shader fragment

layout(location = 0) out vec4 o_Color;

void main() {
    o_Color = vec4(1.0, 0.0, 0.0, 1.0);
}
