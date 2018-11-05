#ifndef _SHADER_H
#define _SHADER_H

#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>

class Shader {
protected:
	GLuint program;
	GLuint vao;
	GLuint textureId;

	void printShaderLog(GLuint shader);
	std::string loadShaderSource(std::string shader);
public:
	Shader() {}

	void init(std::string vertexShaderFile, std::string fragmentShaderFile);
	void useProgram();
	GLuint getProgram() { return program; }
};

#endif