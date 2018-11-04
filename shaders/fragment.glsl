#version 330 core

in vec3 color;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D texMap;

void main() {
	vec4 texel = texture(texMap, texCoord);
	fragColor = texel;
	//fragColor = vec4(color, 1.0);
}