#include "Game.h"
#include "Camera.h"
#include <cstdio>

Game::Game()
{
	m_width = 1280;
	m_height = 720;
	m_title = "Computer Graphics";
}

Game::Game(int width, int height, const char* title)
{
	m_width = width;
	m_height = height;
	m_title = title;
}

Game::~Game()
{
}

int Game::run()
{
	bool updating = true;
	bool drawing = true;

	double deltaTime = 0.0f;
	double timeOfPreviousUpdate = 0.0;

	if (!start())
		return -1;

	while (updating && drawing) {
		//Get the current time
		double timeOfCurrentUpdate = glfwGetTime();
		//Find the change in time
		deltaTime = timeOfCurrentUpdate - timeOfPreviousUpdate;
		//Store the current time for the next loop
		timeOfPreviousUpdate = timeOfCurrentUpdate;

		updating = update(deltaTime);
		drawing = draw();
	}

	if (!end())
		return -2;

	return 0;
}

bool Game::start()
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
	m_camera = new Camera(this);
	m_camera->setPosition({ 10, 10, 10 });
	m_camera->setYaw(-135.0f);
	m_camera->setPitch(-45.0f);

	//Set the clear color
	glClearColor(0.05f, 0.05f, 0.025f, 1.0f);
	//Enable OpenGL depth test
	glEnable(GL_DEPTH_TEST);

	//Create a ball
	m_ball = new Ball({ 0.8f, 0.1f, 0.1f, 1.0f }, 2.0f);
	//Create a placeholder for a starting position and rotation
	m_startActor = new Actor();
	m_startActor->setPosition({ 10.0f, 5.0f, 10.0f });
	m_startActor->setRotation(glm::vec3(0.0f, -1.0f, 1.0f));
	//Create a placeholder for an ending position and rotation
	m_endActor = new Actor();
	m_endActor->setPosition({ -10.0f, 0.0f, -10.0f });
	m_endActor->setRotation(glm::vec3(0.0f, 1.0f, -1.0f));
	//Set the ball's position and rotation to the start
	m_ball->setPosition(m_startActor->getPosition());
	m_ball->setRotation(m_startActor->getRotation());

	return true;
}

bool Game::update(double deltaTime)
{
	glfwPollEvents();

	//Keep the window open until the user closes it
	if (glfwWindowShouldClose(m_window) || glfwGetKey(m_window, GLFW_KEY_ESCAPE)) {
		return false;
	}

	m_camera->update(deltaTime);

	//Find a time-based value in the range of [0, 1]
	float s = glm::cos(glfwGetTime()) * 0.5f + 0.5f;
	//Standard linear interpolation
	glm::vec3 startPosition(m_startActor->getPosition());
	glm::vec3 endPosition(m_endActor->getPosition());
	glm::vec3 p = (1.0f - s) * startPosition + s * endPosition;
	//Quaternion slerp
	glm::quat startRotation(m_startActor->getRotation());
	glm::quat endRotation(m_endActor->getRotation());
	glm::quat r = glm::slerp(startRotation, endRotation, s);
	//Update postion and rotation of the ball
	m_ball->setPosition(p);
	m_ball->setRotation(r);

	return true;
}

bool Game::draw()
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

	m_ball->draw();

	aie::Gizmos::draw(m_camera->getProjectionMatrix(m_width, m_height) * m_camera->getViewMatrix());

	glfwSwapBuffers(m_window);

	return true;
}

bool Game::end()
{
	delete m_ball;

	//Destroy the Gizmos
	aie::Gizmos::destroy();

	//Close the window
	glfwDestroyWindow(m_window);

	//Terminate GLFW
	glfwTerminate();

	return true;
}
