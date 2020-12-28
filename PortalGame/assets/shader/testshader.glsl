#pragma name TestShader

#pragma type vertex
#version 330 core
layout(location = 0) in vec4 aPos;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
}

#pragma type fragment
#version 330 core
out vec4 FragColor;

void main() {
	FragColor = vec4(0.8, 0.1f, 0.4f, 1.0f);
}