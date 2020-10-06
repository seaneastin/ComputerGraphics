#pragma once
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Ball.h"
#include "Bone.h"

class Camera;

class Game
{
public:
	Game();
	Game(int width, int height, const char* title);
	~Game();

	int run();

	bool start();
	bool update(double deltaTime);
	bool draw();
	bool end();

	GLFWwindow* getWindow() { return m_window; }
	int getWidth() { return m_width; }
	int getHeight() { return m_height; }
	const char* getTitle() { return m_title; }

protected:
	GLFWwindow* m_window;
	Camera* m_camera;
	Actor* m_startActor;
	Actor* m_endActor;
	Bone* m_bone;

private:
	int m_width, m_height;
	const char* m_title;
};

