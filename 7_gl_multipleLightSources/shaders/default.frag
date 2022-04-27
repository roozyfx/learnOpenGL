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

vec3 n = normalize(normal);

vec4 pointLight(vec3 light_color, vec3 light_pos)
{
    vec3 lightVec = light_pos - currentPos;
    //distance to the light
    float d = length(lightVec);
    vec3 ray = normalize(lightVec);
    float diffuse = max(dot(n, ray), 0.0);
    float ambient = 0.2;

    vec3 viewDir = normalize(currentPos - cameraPos);
    vec3 reflectDir = reflect(-viewDir, n);
    float specularFact = 0.6;

    float specular = specularFact * pow(max(dot(reflectDir, viewDir), 0), 8);

    vec4 nonAttenuatedLight = vec4(light_color * (ambient + diffuse + specular), 1.0);
    float a = 2.15;
    float b = 0.75;
    float attenuationFactor = 1./(a*d*d + b*d + 1);
    return nonAttenuatedLight * attenuationFactor;
}

vec4 directionalLight(vec3 light_color, vec3 light_direction)
{
    float diffuse = max(dot(n, light_direction), 0);
    vec3 viewDir = normalize(currentPos - cameraPos);
    vec3 reflectDir = reflect(-viewDir, n);
    float specularFact = 0.6;

    float specular = specularFact * pow(max(dot(reflectDir, viewDir), 0), 8);
    return vec4(light_color * (diffuse + specular), 1.0);
}

vec4 spotLight(vec3 light_color, vec3 light_pos, vec3 light_dir, float cosInnerCutOff, float cosOuterCutOff)
{
    vec3 lightVec = light_pos - currentPos;
    //distance to the light
    float d = length(lightVec);
    vec3 ray = normalize(lightVec);
    float diffuse = max(dot(n, ray), 0.0);
    float ambient = 0.2;

    vec3 viewDir = normalize(currentPos - cameraPos);
    vec3 reflectDir = reflect(-viewDir, n);
    float specularFact = 0.6;

    float specular = specularFact * pow(max(dot(reflectDir, viewDir), 0), 8);
    light_dir = normalize(light_dir);
    //correct possible user mistake
    if (cosInnerCutOff<cosOuterCutOff){
        float temp = cosInnerCutOff;
        cosInnerCutOff = cosOuterCutOff;
        cosOuterCutOff = temp;
    }
    float cosRayAngle = dot(-ray, light_dir);
    if (cosRayAngle<cosOuterCutOff){
        diffuse = 0.;
        specular = 0.;
    }
    else if(cosRayAngle>=cosOuterCutOff && cosRayAngle<cosInnerCutOff){
        //float lerp = mix(cosOuterCutOff, cosInnerCutOff, cosRayAngle);
        float lerp = clamp((cosRayAngle-cosOuterCutOff)/(cosInnerCutOff-cosOuterCutOff), 0., 1.);
        diffuse *= .5*lerp;
        specular *= .5*lerp;
    }
    else{
        //float lerp = mix(cosInnerCutOff, 1.0, cosRayAngle);
        float lerp = clamp((cosRayAngle-cosInnerCutOff)/(1.0-cosInnerCutOff), 0., 1.);
        diffuse *= lerp;
        specular *= lerp;
    }

    vec4 nonAttenuatedLight = vec4(light_color * (ambient + diffuse + specular), 1.0);
    float a = 2.15;
    float b = 0.75;
    float attenuationFactor = 1./(a*d*d + b*d + 1);
    return nonAttenuatedLight * attenuationFactor;
}

void main()
{


    fragColor = texture(tex0, texCoord) * (pointLight(lightColor, lightPos)
                + directionalLight( vec3(1.0, .0, 1.0), vec3(1.0, 0., 0.) )
                + spotLight( vec3(1.0, 1.0, 0.0), vec3(0., 0., 2.), vec3(0., 0., -1), 0.92866f, 0.87f ));
}
