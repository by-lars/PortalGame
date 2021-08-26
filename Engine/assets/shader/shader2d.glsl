#pragma name Shader2D

#pragma type vertex
#version 460 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aColor;
layout(location = 3) in mat4 aModel;

uniform mat4 uProj;

out vec2 uv;
out vec3 color;

void main() {
	gl_Position = uProj * aModel * vec4(aPos.x, aPos.y, 0, 1.0f);
	color = aColor;
	uv = aUV;
}

#pragma type fragment
#version 460 core
out vec4 FragColor;

in vec2 uv;
in vec3 color;

uniform sampler2D uTex;

void main() {
	FragColor = vec4(color.x, color.y, color.z, 1.0f);
	FragColor = texture(uTex, uv);
}
