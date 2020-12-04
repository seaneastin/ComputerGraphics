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
/**
 * will run untill the game ends
 */
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


/**
 * runs when application start
 */
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

	//Set the clear color
	glClearColor(0.05f, 0.05f, 0.025f, 1.0f);
	//Enable OpenGL depth test
	glEnable(GL_DEPTH_TEST);

	//Initialize shader
	m_shader.loadShader(
		aie::eShaderStage::VERTEX,
		"Sean.vert"
	);
	m_shader.loadShader(
		aie::eShaderStage::FRAGMENT,
		"Sean.frag"
	);
	if (!m_shader.link()) {
		printf(
			"Shader Error: %s\n",
			m_shader.getLastError()
		);
		return false;
	}

	//Load obj mesh
	if (!m_objMesh.load("soulspear.obj")) {
		printf("Failed to load OBJmesh.\n");
		return false;
	}

	//Load diffuse map
	if (!m_texture.load("soulspear_diffuse.tga")) {
		printf("Failed to load diffuse map.\n");
		return false;
	}

	//Load mormal map
	if (!m_normal.load("soulspear_normal.tga")) {
		printf("Failed to load normal map.\n");
		return false;
	}

	//Load specular map
	if (!m_specular.load("soulspear_specular.tga")) {
		printf("Failed to load specular map.\n");
		return false;
	}


	//Initialize Gizmos
	aie::Gizmos::create(10000, 10000, 10000, 10000);

	//Set up the camera
	m_camera = new Camera(this);
	m_camera->setPosition({ 10, 10, 10 });
	m_camera->setYaw(-135.0f);
	m_camera->setPitch(-45.0f);

	m_earth = new Earth(
		{ 0.0f, 0.0f, 0.0f },
		glm::vec3(0.0f, 0.0f, 0.0f),
		{ 10.0f, 10.0f, 10.0f }
	);
	m_earth->start();

	//Set up the identity transform
	m_meshTransform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	//Create bones
	m_hipBone = new Bone(
		{ { 0.0f, 5.0f, 0.0f }, glm::vec3(1.0f, 0.0f, 0.0f) },
		{ { 0.0f, 5.0f, 0.0f }, glm::vec3(-1.0f, 0.0f, 0.0f) }
	);
	m_kneeBone = new Bone(
		{ { 0.0f, -2.5f, 0.0f }, glm::vec3(1.0f, 0.0f, 0.0f) },
		{ { 0.0f, -2.5f, 0.0f }, glm::vec3(0.0f, 0.0f, 0.0f) }
	);
	m_ankleBone = new Bone(
		{ { 0.0f, -2.5f, 0.0f }, glm::vec3(-1.0f, 0.0f, 0.0f) },
		{ { 0.0f, -2.5f, 0.0f }, glm::vec3(0.0f, 0.0f, 0.0f) }
	);
	m_kneeBone->setParent(m_hipBone);
	m_ankleBone->setParent(m_kneeBone);

	//Create a skeleton
	m_skeleton = new Skeleton();
	//Add the bone to the skeleton
	m_skeleton->addBone(m_hipBone);
	m_skeleton->addBone(m_kneeBone);
	m_skeleton->addBone(m_ankleBone);

	m_light.setAmbient({ 0.5f, 0.5f, 0.5f });
	m_light.setDiffuse({ 1.0f, 1.0f, 1.0f });
	m_light.setSpecular({ 1.0f, 1.0f, 1.0f });

	return true;
}
/**
 * updates the application
 */
bool Game::update(double deltaTime)
{
	glfwPollEvents();

	//Keep the window open until the user closes it
	if (glfwWindowShouldClose(m_window) || glfwGetKey(m_window, GLFW_KEY_ESCAPE)) {
		return false;
	}

	m_camera->update(deltaTime);

	//Rotate the light
	float time = glfwGetTime();
	m_light.setDirection(glm::normalize(glm::vec3(
		glm::cos(time * 2),
		-1,
		glm::sin(time * 2))
	));

	//m_skeleton->update(deltaTime);

	return true;
}

/**
 * draws the scene
 */
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

	//Get the projection and view matrices
	mat4 projectionMatrix = m_camera->getProjectionMatrix(m_width, m_height);
	mat4 viewMatrix = m_camera->getViewMatrix();

	//Bind shader
	m_shader.bind();

	//Bind camera
	m_shader.bindUniform("CameraPosition", m_camera->getPosition());

	//Bind light
	m_shader.bindUniform("Ia", m_light.getAmbient());
	m_shader.bindUniform("Id", m_light.getDiffuse());
	m_shader.bindUniform("Is", m_light.getSpecular());
	m_shader.bindUniform("LightDirection", m_light.getDirection());

	//Bind and draw Earth
	mat4 pvm = projectionMatrix * viewMatrix * m_earth->getTransform();
	m_shader.bindUniform("ProjectionViewModel", pvm);
	m_shader.bindUniform("NormalMatrix",
		glm::inverseTranspose(glm::mat3(m_earth->getTransform())));
	m_shader.bindUniform("ModelMatrix", m_earth->getTransform());
	m_shader.bindUniform("diffuseTexture", 0);
	m_earth->draw();

	//Draw obj mesh
	pvm = projectionMatrix * viewMatrix * m_meshTransform;
	m_shader.bindUniform("ProjectionViewModel", pvm);
	//m_shader.bindUniform("diffuseTexture", 0);
	m_objMesh.draw();

	//m_skeleton->draw();

	aie::Gizmos::draw(projectionMatrix * viewMatrix);

	glfwSwapBuffers(m_window);

	return true;
}

/**
 * called when the game ends
 */
bool Game::end()
{
	delete m_hipBone;
	delete m_kneeBone;
	delete m_ankleBone;
	delete m_skeleton;

	//Destroy the Gizmos
	aie::Gizmos::destroy();

	//Close the window
	glfwDestroyWindow(m_window);

	//Terminate GLFW
	glfwTerminate();

	return true;
}
