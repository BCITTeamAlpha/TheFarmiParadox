#version 330 es

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
out vec4 v_color;
out vec3 v_normal;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;
uniform bool passThrough;
uniform bool shadeInFrag;

void main()
{
    if (passThrough)
    {
        // Simple passthrough shader
        v_color = color;
        v_normal = vec3(0, 0, 0);
    } else if (shadeInFrag) {
        v_normal = normal;
    } else {
        // Diffuse shading
        vec3 eyeNormal = normalize(normalMatrix * normal);
        vec3 lightPosition = vec3(0.0, 0.0, 1.0);
        vec4 diffuseColor = color;
        
        float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
        
        v_color = diffuseColor * nDotVP;
    }

    gl_Position = modelViewProjectionMatrix * position;
}