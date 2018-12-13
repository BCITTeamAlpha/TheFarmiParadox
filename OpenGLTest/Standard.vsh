#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_texCoord;
out vec3 v_position;
out vec4 v_lightSpace_position;
out vec3 v_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpace;

void main() {
	v_normal = normalize(view * model * vec4(normal, 0.0)).xyz;
	v_texCoord = texCoord;

	vec4 model_space_position = model * vec4(position, 1.0);
	vec4 light_space_position = lightSpace * model_space_position;
	vec4 view_space_position = view * model_space_position;

	v_position = view_space_position.xyz;
	v_lightSpace_position = light_space_position;
    gl_Position = projection * view_space_position;
}