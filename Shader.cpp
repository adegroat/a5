#include "Shader.h"

void Shader::useProgram() {
	glUseProgram(program);
}

std::string Shader::loadShaderSource(std::string shader) {
	std::ifstream shaderFile(shader.c_str());
	if(!shaderFile.is_open()) {
		std::cout << "Failed to open shader file " << shader << std::endl;
		return "";
	}

	std::string data;

	while(!shaderFile.eof()) {
		std::string line;
		std::getline(shaderFile, line);
		data += line + "\n";
	}

	return data;
}

void Shader::printShaderLog(GLuint shader) {
	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "Shader " << shader << " compile error\n" << infoLog << std::endl;
	} else {
		std::cout << "Shader " << shader << " successfully compiled." << std::endl;
	}
}

void Shader::init(std::string vertexShaderFile, std::string fragmentShaderFile) {
	const char* vertexShaderSource = loadShaderSource(vertexShaderFile).c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	printShaderLog(vertexShader);

	const char* fragmentShaderSource = loadShaderSource(fragmentShaderFile).c_str();
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	printShaderLog(fragmentShader);
	

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);


	// Print status of program linking
	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "Shader program link error:\n" << infoLog << std::endl;
	}
	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}