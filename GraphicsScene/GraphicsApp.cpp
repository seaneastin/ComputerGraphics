#include "GraphicsApp.h"
#include <cstdio>

GraphicsApp::GraphicsApp()
{
	m_width = 1280;
	m_height = 720;
	m_title = "Computer Graphics";
}

GraphicsApp::GraphicsApp(int width, int height, const char* title)
{
	m_width = width;
	m_height = height;
	m_title = title;
}

GraphicsApp::~GraphicsApp()
{
}

int GraphicsApp::run()
{
	bool updating = true;
	bool drawing = true;

	if (!start())
		return -1;

	while (updating && drawing) {
		updating = update();
		drawing = draw();
	}

	if (!end())
		return -2;

	return 0;
}

bool GraphicsApp::start()
{
	using glm::vec3;
	using glm::vec4;
	using glm::mat4;

	//Initialize GLFW
	if (!glfwInit()) {
		return false;
	}

	//Create a window
	m_window = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

	//Ensure the window was created
	if (!m_window) {
		glfwTerminate();
		return false;
	}

	//Set the window as our target
	glfwMakeContextCurrent(m_window);

	//Load OpenGL functions
	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	//Print the OpenGL version number
	int major = ogl_GetMajorVersion();
	int minor = ogl_GetMinorVersion();
	printf("OpenGL version: %i.%i\n", major, minor);

	//Initialize Gizmos
	aie::Gizmos::create(10000, 10000, 10000, 10000);

	//Set up the camera
	m_view = glm::lookAt(
		vec3(10, 10, 10),
		vec3(0, 0, 0),
		vec3(0, 1, 0));
	m_projection = glm::perspective(
		glm::pi<float>() * 0.25f,
		16.0f / 9.0f,
		0.1f, 1000.0f);

	//Set the clear color
	glClearColor(0.05f, 0.05f, 0.025f, 1.0f);
	//Enable OpenGL depth test
	glEnable(GL_DEPTH_TEST);

	return true;
}

bool GraphicsApp::update()
{
	glfwPollEvents();

	//Keep the window open until the user closes it
	if (glfwWindowShouldClose(m_window) || glfwGetKey(m_window, GLFW_KEY_ESCAPE)) {
		return false;
	}

	return true;
}

bool GraphicsApp::draw()
{
	using glm::vec3;
	using glm::vec4;
	using glm::mat4;

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

	aie::Gizmos::draw(m_projection * m_view);

	glfwSwapBuffers(m_window);

	return true;
}

bool GraphicsApp::end()
{
	//Destroy the Gizmos
	aie::Gizmos::destroy();

	//Close the window
	glfwDestroyWindow(m_window);

	//Terminate GLFW
	glfwTerminate();

	return true;
}
