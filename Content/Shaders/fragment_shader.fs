#version 450 core

uniform sampler2D texture1;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec4 fragDiffuse;

out vec4 outColor;

void main() {
    outColor = texture(texture1, fragTexCoord) * fragDiffuse;
}