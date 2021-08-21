#pragma name Shader2D

#pragma type vertex
#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 uModel;
uniform mat4 uProj;

out vec3 color;

void main() {
	gl_Position = uProj * uModel * vec4(aPos.x, aPos.y, 0, 1.0f);
	color = aColor;
}

#pragma type fragment
#version 460 core
out vec4 FragColor;
in vec3 color;

void main() {
	FragColor = vec4(color.x, color.y, color.z, 1.0f);
}