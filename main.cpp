#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>

int windowWidth = 800, windowHeight = 600;

void errorCallback(int error, const char* desc) {
	std::cout << "ERROR: " << desc << std::endl;
}

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_ESCAPE:
			case GLFW_KEY_Q:
				glfwSetWindowShouldClose(window, GLFW_TRUE);
				break;
		}
	}
}

void cursorCallback( GLFWwindow *window, double x, double y ) {
	
}

void mouseButtonCallback( GLFWwindow *window, int button, int action, int mods ) {

}

GLFWwindow* setupGLFW() {
	glfwSetErrorCallback(errorCallback);

	if(!glfwInit()) {
		std::cout << "Error: Could not initialize GLFW" << std::endl;
		return NULL;
	}

	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow *window = glfwCreateWindow( windowWidth, windowHeight, "A5", NULL, NULL );
	if(!window) {
		std::cout << "Error: Could not create window" << std::endl;
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetCursorPosCallback(window, cursorCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	return window;
}

void setupOpenGL() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
}

void setupShaders() {

}

void render() {

}

void update() {

}

int main( int argc, char *argv[] ) {
	if(argc != 2) {
		std::cout << "Correct usage: ./a5 MODEL_FILE.obj" << std::endl;
		return -1;
	}

	GLFWwindow *window = setupGLFW();
	setupOpenGL();

	setupShaders();

	while(!glfwWindowShouldClose(window)) {
		glDrawBuffer( GL_BACK );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		glm::mat4 projMtx = glm::perspective( 45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.001f, 1000.0f );

		GLint framebufferWidth, framebufferHeight;
		glfwGetFramebufferSize( window, &framebufferWidth, &framebufferHeight );

		glViewport( 0, 0, framebufferWidth, framebufferHeight );

		glm::mat4 viewMtx = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
										glm::vec3(0.0f, 0.0f, 0.0f),
										glm::vec3(0.0f, 1.0f, 0.0f));

		render();
		update();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow( window );
	glfwTerminate();

	return 0;
}