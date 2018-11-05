#version 330 core

in vec3 vPos;
in vec3 normal;

out vec4 theColor;

uniform mat4 mvpMatrix;

void main() {
	theColor = vec4(vPos, 1.0);
	gl_Position = mvpMatrix * vec4(vPos, 1.0);
}