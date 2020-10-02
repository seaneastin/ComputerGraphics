#pragma once
#include <glm/glm.hpp>

class Actor
{
public:
	Actor() {}
	~Actor() {}

	virtual bool update(double deltaTime) { return true; }
	virtual bool draw() { return true; }

protected:
	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };
};

