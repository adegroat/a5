#version 330 core

layout (location = 0) in vec3 vPos;
//layout (location = 1) in vec3 textureCoordinate;

out vec3 texCoord;

uniform mat4 mvpMatrix;

void main() {
	gl_Position = mvpMatrix * vec4(vPos, 1.0);
	//texCoord = vec3(textureCoordinate.x, 1.0 - textureCoordinate.y, textureCoordinate.z);
	texCoord = vPos;
}