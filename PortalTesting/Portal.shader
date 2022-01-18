#shader shared

#version 440 core

uniform mat4 u_ModelMatrix = mat4(1.0);
uniform mat4 u_ViewMatrix = mat4(1.0);
uniform mat4 u_ProjectionMatrix = mat4(1.0);

uniform vec4 u_Color = vec4(1.0f);
uniform sampler2D u_Texture;
uniform vec2 u_ScreenSize = vec2(0.0f);

#shader vertex

layout(location = 0) in vec4 a_Position;

void main() {
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * a_Position;
}

#shader fragment

layout(location = 0) out vec4 o_Color;

void main() {
    o_Color = texture(u_Texture, gl_FragCoord.xy / u_ScreenSize) * u_Color;
}
