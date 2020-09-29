#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <cstdio>

using glm::vec3;
using glm::vec4;
using glm::mat4;

int main() {
	//Initialize GLFW
	if (!glfwInit()) {
		return -1;
	}

	//Create a window
	GLFWwindow* window =
		glfwCreateWindow(1280, 720,
		"Computer Graphics",
		nullptr, nullptr);
	
	//Ensure the window was created
	if (!window) {
		glfwTerminate();
		return -2;
	}

	//Set the window as our target
	glfwMakeContextCurrent(window);

	//Load OpenGL functions
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return -3;
	}

	//Print the OpenGL version number
	int major = ogl_GetMajorVersion();
	int minor = ogl_GetMinorVersion();
	printf("OpenGL version: %i.%i\n", major, minor);

	//Initialize Gizmos
	aie::Gizmos::create(10000, 10000, 10000, 10000);

	//Set up the camera
	mat4 view = glm::lookAt(
		vec3(10, 10, 10),
		vec3(0, 0, 0),
		vec3(0, 1, 0));
	mat4 projection = glm::perspective(
		glm::pi<float>() * 0.25f,
		16.0f / 9.0f,
		0.1f, 1000.0f);

	//Set the clear color
	glClearColor(0.05f, 0.05f, 0.025f, 1.0f);
	//Enable OpenGL depth test
	glEnable(GL_DEPTH_TEST);

	//Keep the window open until the user closes it
	while (!glfwWindowShouldClose(window) &&
		!glfwGetKey(window, GLFW_KEY_ESCAPE)) {

		//Clear the window
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Clear the Gizmos
		aie::Gizmos::clear();
		
		aie::Gizmos::addTransform(mat4(1), 4.0f);

		vec4 white(1, 1, 1, 1);
		vec4 grey(0.5f, 0.5f, 0.5f, 1);

		for (int i = 0; i < 21; ++i) {
			aie::Gizmos::addLine(
				vec3(-10 + i, 0, 10),
				vec3(-10 + i, 0, -10),
				i == 10 ? white : grey);
			aie::Gizmos::addLine(
				vec3(10, 0, -10 + i),
				vec3(-10, 0, -10 + i),
				i == 10 ? white : grey);
		}

		aie::Gizmos::draw(projection * view);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Destroy the Gizmos
	aie::Gizmos::destroy();

	//Close the window
	glfwDestroyWindow(window);

	//Terminate GLFW
	glfwTerminate();
	return 0;
}