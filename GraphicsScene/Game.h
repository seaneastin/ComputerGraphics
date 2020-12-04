#pragma once
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Gizmos.h>
#include <Texture.h>
#include "OBJMesh.h"
#include "Shader.h"
#include "Mesh.h"
#include "Light.h"
#include "Ball.h"
#include "Bone.h"
#include "Skeleton.h"
#include "Earth.h"

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
	GLFWwindow*	m_window;
	Camera*		m_camera;

	aie::ShaderProgram	m_shader;

	Light m_light;

	aie::OBJMesh		m_objMesh;
	aie::Texture		m_objTexture;
	glm::mat4			m_meshTransform;

	Earth* m_earth;

	Mesh				m_mesh;
	aie::Texture		m_texture;
	aie::Texture		m_normal;
	aie::Texture		m_specular;

	Bone*		m_hipBone;
	Bone*		m_kneeBone;
	Bone*		m_ankleBone;
	Skeleton*	m_skeleton;

private:
	int m_width, m_height;
	const char* m_title;
};

