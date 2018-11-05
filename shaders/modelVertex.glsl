#version 330 core

in vec3 vPos;
in vec3 vNormal;

out vec3 theColor;

uniform mat4 mvpMatrix;
uniform vec3 lightPos;
uniform vec3 camPos;

void main() {
	gl_Position = mvpMatrix * vec4(vPos, 1.0);

	vec3 lightColor = vec3(1.0, 1.0, 1.0);

	vec3 ambient = 0.3 * lightColor;

	vec3 norm = normalize(vNormal);
	vec3 lightDir = normalize(lightPos - vPos);
	vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightColor;

	vec3 camDir = normalize(camPos - vPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	vec3 specular = 0.5 * pow(max(dot(camDir, reflectDir), 0.0), 128) * lightColor;

	theColor = (ambient + diffuse + specular) * lightColor;
}