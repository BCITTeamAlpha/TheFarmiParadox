#version 330 es

precision highp float;
in vec4 v_color;
in vec3 v_normal;
out vec4 o_fragColor;

uniform mat3 normalMatrix;
uniform bool passThrough;
uniform bool shadeInFrag;

void main()
{
    if (!passThrough && shadeInFrag) {
        vec3 eyeNormal = normalize(normalMatrix * v_normal);
        vec3 lightPosition = vec3(0.0, 0.0, 1.0);
        vec4 diffuseColor = v_color;
        
        float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
        
        o_fragColor = diffuseColor * nDotVP;
    } else {
        o_fragColor = v_color;
    }
}