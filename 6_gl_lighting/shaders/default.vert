#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

uniform mat4 model;
uniform mat4 cameraMat;

out vec3 color;
out vec2 texCoord;
out vec3 normal;

out vec3 currentPos;

void main()
{
    gl_Position = cameraMat * vec4(aPos, 1.0);
    currentPos = vec3(model * vec4(aPos, 1.0));
    color = aColor;
    texCoord = aTexCoord;
    normal = aNormal;
}
