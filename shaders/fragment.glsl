#version 330 core

in vec3 texCoord;

out vec4 fragColor;

uniform samplerCube cubeTexMap;

void main() {
	vec4 texel = texture(cubeTexMap, texCoord);

	fragColor = texel * 0.6;
}