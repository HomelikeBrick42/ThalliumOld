#shader shared

#version 440 core

uniform mat4 u_ModelMatrix = mat4(1.0);
uniform mat4 u_ViewMatrix = mat4(1.0);
uniform mat4 u_ProjectionMatrix = mat4(1.0);

#shader vertex

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec3 a_Normal;

layout(location = 0) out vec3 v_Normal;

void main() {
    v_Normal = (u_ModelMatrix * vec4(a_Normal, 0.0)).xyz;
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * a_Position;
}

#shader fragment

layout(location = 0) out vec4 o_Color;

layout(location = 0) in vec3 v_Normal;

void main() {
    o_Color = (dot(normalize(v_Normal), normalize(vec3(0.3, 0.5, 0.4))) * 0.5 + 0.5) * vec4(1.0, 0.0, 0.0, 1.0);
}
