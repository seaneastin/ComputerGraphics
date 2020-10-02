#include "Ball.h"
#include <Gizmos.h>

Ball::Ball()
{
	m_color = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	m_radius = 1.0f;
}

Ball::Ball(glm::vec4 color, float radius)
{
	m_color = color;
	m_radius = radius;
}

bool Ball::draw()
{
	aie::Gizmos::addSphere(m_position, m_radius, 6 * m_radius, 6 * m_radius, m_color);

	return true;
}
