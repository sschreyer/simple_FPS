#version 330 core

in  vec3 Color;
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main() {
    FragColor = texture(ourTexture, TexCoords) * vec4(Color, 1.0);
}


