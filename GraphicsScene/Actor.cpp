#include "Actor.h"

Actor::Actor(glm::vec3 position, glm::quat rotation)
{
    m_position = position;
    m_rotation = rotation;
}

glm::mat4 Actor::getTransform()
{
    if (m_parent) {
        return m_parent->getTransform() * glm::translate(m_position) * glm::toMat4(m_rotation);
    }

    return glm::translate(m_position) * glm::toMat4(m_rotation);
}

glm::vec3 Actor::getParentPosition()
{
    if (m_parent) {
        return m_parent->getPosition();
    }

    return glm::vec3(0.0f);
}

glm::quat Actor::getParentRotation()
{
    if (m_parent) {
        return m_parent->getRotation();
    }

    return glm::quat(glm::vec3(0.0f));
}
