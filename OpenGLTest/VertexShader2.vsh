#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec4 v_color;
out vec2 v_texCoord;

uniform mat4 model;
uniform mat4 viewProjection;
uniform vec4 u_color = vec4(1.0);

void main() {
	v_color = u_color;
	v_texCoord = texCoord;
	gl_Position = viewProjection * model * vec4(position, 1.0);
}