#version 330 core

in vec3 vPos;

out vec3 color;

uniform mat4 mvpMatrix;

void main() {
	color = vPos;
	gl_Position = mvpMatrix * vec4(vPos, 1.0);
}