#pragma once
#include <glm/glm.hpp>

class Mesh
{
public:
	Mesh() : m_triCount(0), m_vao(0), m_vbo(0), m_ibo(0) {}
	~Mesh();

	void initializeQuad();

	virtual void draw();

public:
	struct Vertex {
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec4 color;
		glm::vec2 texCoord;
	};

protected:
	unsigned int m_triCount;
	unsigned int m_vao, m_vbo, m_ibo;
};

