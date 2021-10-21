#version 330 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

out vec3 FragPos;
out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 pv;

void main() {
    gl_Position = pv * model * vec4(vert_pos, 1.0f);
    FragPos = vec3(model * vec4(vert_pos, 1.0));
    Normal = mat3(transpose(inverse(model))) * inNormal;
    TexCoords = inTexCoords;
}
