#version 460 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(.5*aPos.x, 0.5*aPos.y, 0.5*aPos.z, 1.0);
}
