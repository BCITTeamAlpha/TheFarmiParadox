#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 normal;

out vec4 v_color;
out vec3 v_position;
out vec3 v_normal;

uniform mat4 modelView;
uniform mat4 projection;

void main()
{
    v_color = vec4(1.0, 0.0, 0.0, 1.0);
	//v_color = color;
	v_normal = normal;
	v_position = (modelView * position).xyz;
    gl_Position = projection * modelView * position;
}