#include "Mesh.h"
#include <gl_core_4_4.h>

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ibo);
}

void Mesh::initialize(
	unsigned int vertexCount, const Vertex* vertices,
	unsigned int indexCount, unsigned int* indices)
{
	//Check that the mesh is not already initialized
	assert(m_vao == 0);

	//Generate buffers
	glGenBuffers(1, &m_vbo);
	glGenVertexArrays(1, &m_vao);

	//Bind buffers
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	//Fill vertex buffer
	glBufferData(
		GL_ARRAY_BUFFER,
		vertexCount * sizeof(Vertex),
		vertices,
		GL_STATIC_DRAW
	);

	//Enable first element as position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		0
	);
	//Enable second element as color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(GLvoid*)(8 * sizeof(float))
	);

	//Bind indices if there are any
	if (indexCount != 0) {
		//Generate index buffer
		glGenBuffers(1, &m_ibo);

		//Bind index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		//Fill index buffer
		glBufferData(
			GL_ELEMENT_ARRAY_BUFFER,
			indexCount * sizeof(unsigned int),
			indices,
			GL_STATIC_DRAW
		);

		m_triCount = indexCount / 3;
	}
	else {
		m_triCount = vertexCount / 3;
	}

	//Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::initializeQuad()
{
	//Define 6 vertices for 2 triangles
	Vertex vertices[6];
	//Triangle A
	vertices[0].position = { -0.5f, 0.0f, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0.0f, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0.0f, -0.5f, 1 };
	//Triangle B
	vertices[3].position = { -0.5f, 0.0f, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0.0f, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0.0f, -0.5f, 1 };
	//Triangle A
	vertices[0].color = { 0.2f, 0.2f, 0.8f, 1 };//blue
	vertices[1].color = { 0.8f, 0.8f, 0.2f, 1 };//yellow
	vertices[2].color = { 0.2f, 0.8f, 0.8f, 1 };//cyan
	//Triangle B
	vertices[3].color = { 0.2f, 0.8f, 0.2f, 1 };//green
	vertices[4].color = { 0.8f, 0.2f, 0.8f, 1 };//magenta
	vertices[5].color = { 0.8f, 0.2f, 0.2f, 1 };//red

	initialize(6, vertices);
}

void Mesh::draw()
{
	glBindVertexArray(m_vao);
	//Using indices or just vertices
	if (m_ibo != 0)
		glDrawElements(
			GL_TRIANGLES,
			3 * m_triCount,
			GL_UNSIGNED_INT, 0
		);
	else
		glDrawArrays(
			GL_TRIANGLES,
			0,
			3 * m_triCount
		);
}
