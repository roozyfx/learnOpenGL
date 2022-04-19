#version 460 core

in vec3 color;
in vec2 texCoord;
in vec3 normal;

in vec3 currentPos;

uniform vec3 cameraPos;

uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 fragColor;

uniform sampler2D tex0;

void main()
{
    vec4 fColor = vec4(color, 1.0f);
    vec3 n = normalize(normal);
    vec3 ray = normalize(lightPos - currentPos);
    float diffuse = max(dot(n, ray), 0.0);
    float ambient = 0.2;

    vec3 viewDir = normalize(currentPos - cameraPos);
    vec3 reflect = reflect(-viewDir, n);
    float specularFact = 0.6;

    float specular = specularFact * pow(max(dot(reflect, viewDir), 0), 8);


    vec4 light = vec4(lightColor * (ambient + diffuse + specular), 1.0);

    fragColor = texture(tex0, texCoord) * light;
//    fragColor = fColor * light;
}
