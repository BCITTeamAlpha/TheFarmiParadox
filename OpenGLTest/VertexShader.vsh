#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;
out vec4 v_color;
out vec3 v_normal;

uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

void main()
{
	//v_color = color;
    v_color = vec4(1.0, 0.0, 0.0, 1.0);
    
	//v_normal = normal;
	v_normal = vec3(0, 0, 0);

    //gl_Position = modelViewProjectionMatrix * position;
	gl_Position = position;
}