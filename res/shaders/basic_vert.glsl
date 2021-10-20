#version 330 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 inTexCoords;

out vec2 TexCoords;

uniform mat4 mvp;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = mvp * vec4(vert_pos, 1.0f);
    TexCoords = inTexCoords;
}
