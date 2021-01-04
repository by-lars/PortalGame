#pragma name TestShader

#pragma type vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;


out vec3 pos;
out vec3 col;

void main() {
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
	pos = aPos;
	col = aCol;
}

#pragma type fragment
#version 330 core
out vec4 FragColor;
in vec3 pos;
in vec3 col;

void main() {
	//FragColor = vec4(0.8, 0.1f, 0.4f, 1.0f);
	FragColor = vec4(col.x, col.y, col.z, 1.0f);
}