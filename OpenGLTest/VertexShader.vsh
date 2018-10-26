#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_texCoord;
out vec3 v_position;
out vec3 v_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	v_normal = normalize(view * model * vec4(normal, 0.0)).xyz;
	v_texCoord = texCoord;

	vec4 temp_position = view * model * vec4(position, 1.0);
	v_position = temp_position.xyz;
    gl_Position = projection * temp_position;
}