#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <SOIL/SOIL.h>

#include <stdio.h>
#include <string>
#include <map>

#include <CSCI441/modelLoader3.hpp>

#include "Shader.h"

int windowWidth = 800;
int windowHeight = 600;

// Max frame rate
float fps = 60.0f;

// Mouse and keyboard inputs
std::map<int, bool> keyboard;
bool leftMouse = false;
glm::vec2 mousePos;

// Sky box 
GLuint skyboxVao;
GLuint skyboxTex;
Shader skyboxShader;

// Model
CSCI441::ModelLoader* objModel;
GLuint modeVao;
Shader modelShader;

// Camera variables
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camPos;
glm::vec3 camForward;
float yaw, pitch;
float moveSpeed = 5.0f;

void errorCallback(int error, const char* desc) {
	std::cout << "ERROR: " << desc << std::endl;
}

void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if(action == GLFW_PRESS) {
		if(key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		} else {
			keyboard[key] = true;
		}
	}
	if(action == GLFW_RELEASE) {
		keyboard[key] = false;
	}
}

void cursorCallback( GLFWwindow *window, double x, double y ) {
	float mdx = x - mousePos.x;
	float mdy = y - mousePos.y;

	if(leftMouse) {
		yaw += 0.008f * mdx;
		pitch += 0.008f * mdy;
	}

	mousePos.x = x;
	mousePos.y = y;
}

void mouseButtonCallback( GLFWwindow *window, int button, int action, int mods ) {
	if(action == GLFW_PRESS) {
		if(button == GLFW_MOUSE_BUTTON_LEFT) leftMouse = true;
	}
	if(action == GLFW_RELEASE) {
		if(button == GLFW_MOUSE_BUTTON_LEFT) leftMouse = false;
	}
}

// Sets up the GLFW window and registers all mouse/keyboard/error callbacks
GLFWwindow* setupGLFW() {
	glfwSetErrorCallback(errorCallback);

	if(!glfwInit()) {
		std::cout << "Error: Could not initialize GLFW" << std::endl;
		return NULL;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

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


// Sets up all the OpenGL stuff.
void setupOpenGL() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
}

// Initializes GLEW
void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum glewResult = glewInit();

	/* check for an error */
	if( glewResult != GLEW_OK ) {
		printf( "[ERROR]: Error initalizing GLEW\n");
		/* Problem: glewInit failed, something is seriously wrong. */
  		fprintf( stderr, "[ERROR]: %s\n", glewGetErrorString(glewResult) );
		exit(EXIT_FAILURE);
	} else {
		 fprintf( stdout, "[INFO]: GLEW initialized\n" );
		 fprintf( stdout, "[INFO]: Status: Using GLEW %s\n", glewGetString(GLEW_VERSION) );
	}

	if( !glewIsSupported( "GL_VERSION_2_0" ) ) {
		printf( "[ERROR]: OpenGL not version 2.0+.  GLSL not supported\n" );
		exit(EXIT_FAILURE);
	}
}

void setupSkybox() {
	skyboxShader.init("shaders/vertex.glsl", "shaders/fragment.glsl");

	// List of texture names. Must be in this order to look good.
	std::string textures[] = {"posx", "negx", "posy", "negy", "posz", "negz"};

	glGenTextures(1, &skyboxTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

	// Load all the textures and send them to the GPU.
	int width, height;
	unsigned char* textureData = 0;
	for(int i = 0; i < 6; i++) {
		std::string texturePath = "textures/skybox/light_" + textures[i] + ".png";

		textureData = SOIL_load_image(texturePath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
			
		if(textureData == NULL) {
			std::cout << "Failed to load texture: " << texturePath << std::endl;
			continue;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
	}

	// Setup texture parameters
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	float skyboxVerts[] = {
		// Back
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,


		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	glGenVertexArrays(1, &skyboxVao);
	glBindVertexArray(skyboxVao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVerts), skyboxVerts, GL_STATIC_DRAW);

	// Vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

void render() {
	// Draw the sky box
	skyboxShader.useProgram();
	glBindVertexArray(skyboxVao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Draw the model
	modelShader.useProgram();
	GLint vertexPosition = glGetAttribLocation(modelShader.getProgram(), "vPos");
	//GLint normalPosition = glGetAttribLocation(modelShader.getProgram(), "verNormAttribName");
	objModel->draw(vertexPosition);
}

void update(float dt) {
	camForward.x = cos(yaw) * cos(pitch);
	camForward.y = sin(pitch);
	camForward.z = sin(yaw) * cos(pitch);
	camForward = glm::normalize(camForward);
	glm::vec3 strafe = glm::cross(upVector, camForward);

	// WASD Movement
	if(keyboard[GLFW_KEY_W]) {
		camPos += camForward * moveSpeed * dt;
	}
	if(keyboard[GLFW_KEY_S]) {
		camPos -= camForward * moveSpeed * dt;
	}
	if(keyboard[GLFW_KEY_A]) {
		camPos += strafe * moveSpeed * dt;
	}
	if(keyboard[GLFW_KEY_D]) {
		camPos -= strafe * moveSpeed * dt;
	}

	// Arrow keys for looking around
	if(keyboard[GLFW_KEY_LEFT]) {
		yaw -= 1.0f * dt;
	}
	if(keyboard[GLFW_KEY_RIGHT]) {
		yaw += 1.0f * dt;
	}
	if(keyboard[GLFW_KEY_UP]) {
		pitch += 1.0f * dt;
	}
	if(keyboard[GLFW_KEY_DOWN]) {
		pitch -= 1.0f * dt;
	}

	// Limit pitch rotation
	if(pitch >= M_PI/2 - 0.01f) pitch = M_PI/2 - 0.01f;
	if(pitch <= -M_PI/2 + 0.01f) pitch = -M_PI/2 + 0.01f;
}

int main( int argc, char *argv[] ) {
	if(argc != 2) {
		std::cout << "Correct usage: ./a5 OBJ_FILE.obj" << std::endl;
		return -1;
	}

	// Initialize camera
	camPos = glm::vec3(0.0f, 0.0f, 5.0f);
	camForward = glm::vec3(0.0f, 0.0f, -1.0f);
	yaw = -M_PI / 2.0f;
	pitch = 0.0f;

	GLFWwindow *window = setupGLFW();
	setupOpenGL();
	setupGLEW();
	
	setupSkybox();

	// Initialize model
	modelShader.init("shaders/modelVertex.glsl", "shaders/modelFragment.glsl");
	CSCI441::ModelLoader model(argv[1]);
	objModel = &model;

	float prevTime = glfwGetTime();

	while(!glfwWindowShouldClose(window)) {
		// Calculate the time per frame
		float dt = glfwGetTime() - prevTime;
		if(dt > 1.0f/fps) {
			dt = 1.0f/fps;
		}
		prevTime = glfwGetTime();

		glDrawBuffer(GL_BACK);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projMtx = glm::perspective( 45.0f, (GLfloat)windowWidth / (GLfloat)windowHeight, 0.001f, 1000.0f );

		glViewport(0, 0, windowWidth, windowHeight);

		glm::mat4 viewMtx = glm::lookAt(camPos, camPos + camForward, upVector);

		glm::mat4 modelMtx = glm::mat4(1.0f);

		// Calculate the model view projection matrix and send it to the GPU
		glm::mat4 mvpMtx = projMtx * viewMtx * modelMtx;
		int mvpLocation = glGetUniformLocation(skyboxShader.getProgram(), "mvpMatrix");
		skyboxShader.useProgram();
		glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvpMtx[0][0]);

		int modelMvpLocation = glGetUniformLocation(modelShader.getProgram(), "mvpMatrix");
		modelShader.useProgram();
		glUniformMatrix4fv(modelMvpLocation, 1, GL_FALSE, &mvpMtx[0][0]);

		render();
		update(dt);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow( window );
	glfwTerminate();

	return 0;
}