#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 textureCoordinate;

out vec3 color;
out vec2 texCoord;

uniform mat4 mvpMatrix;

void main() {
	gl_Position = mvpMatrix * vec4(vPos, 1.0);
	color = vPos;
	texCoord = vec2(textureCoordinate.x, 1.0 - textureCoordinate.y);
}