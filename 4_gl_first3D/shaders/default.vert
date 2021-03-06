#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    // Pay attention to the order p x v x m
    gl_Position = proj*view*model* vec4(aPos, 1.0);
    color = aColor;
    texCoord = aTexCoord;
}
