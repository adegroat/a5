#version 330 core

layout (location = 0) in vec3 vPos;

out vec3 texCoord;

uniform mat4 mvpMatrix;

void main() {
	float scale = 40.0;
	vec3 scalePos = scale * vPos;

	texCoord = vPos;
	gl_Position = mvpMatrix * vec4(scalePos, 1.0);
}