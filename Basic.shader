#shader shared

#version 410 core

#shader vertex

layout(location = 0) in vec4 a_Position;

void main() {
    gl_Position = a_Position;
}

    #shader fragment

layout(location = 0) out vec4 o_Color;

void main() {
    o_Color = vec4(1.0, 0.0, 0.0, 1.0);
}
