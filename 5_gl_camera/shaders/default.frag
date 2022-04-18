#version 460 core

in vec3 color;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D tex0;

void main()
{
    fragColor = vec4(color, 1.0f);
//    fragColor = texture(tex0, texCoord);
}
