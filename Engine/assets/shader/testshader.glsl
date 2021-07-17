#pragma name TestShader

#pragma type vertex
#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNorm;
layout(location = 3) in mat4 aModel;

uniform mat4 uView;
uniform mat4 uProj;

out vec3 pos;
out vec2 uv;
out vec3 norm;

void main() {
	gl_Position = uProj * uView * aModel * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	pos = aPos;
	uv = aUV;
	norm = aNorm;
}

#pragma type fragment
#version 460 core
out vec4 FragColor;
in vec3 pos;
in vec2 uv;
in vec3 norm;

void main() {
	//FragColor = vec4(0.8, 0.1f, 0.4f, 1.0f);
	FragColor = vec4(norm.x, norm.y, norm.z, 1.0f);
}