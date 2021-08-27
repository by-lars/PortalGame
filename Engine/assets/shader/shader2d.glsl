#pragma name Shader2D

#pragma type vertex
#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;

uniform mat4 uProj;

out vec2 uv;

void main() {
	gl_Position = uProj * vec4(aPos.x, aPos.y, 0.0, 1.0);
	uv = aUV;
}

#pragma type fragment
#version 460 core

out vec4 FragColor;
in vec2 uv;
uniform sampler2D uTex;

void main() {
	FragColor = vec4(1.0);
}
