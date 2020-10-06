#include "Actor.h"

Actor::Actor(glm::vec3 position, glm::quat rotation)
{
    m_position = position;
    m_rotation = rotation;
}

glm::mat4 Actor::getTransform()
{
    return glm::translate(m_position) * glm::toMat4(m_rotation);
}
