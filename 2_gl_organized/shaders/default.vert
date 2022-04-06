#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 color;

uniform float scale;
uniform vec3 transform;

void main()
{
    gl_Position = vec4(aPos.x*scale + transform.x,
                       aPos.y*scale + transform.y,
                       aPos.z*scale + transform.z, 1.0);
    color = aColor;
}
