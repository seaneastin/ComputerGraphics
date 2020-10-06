#include "Actor.h"

glm::mat4 Actor::getTransform()
{
    return glm::translate(m_position) * glm::toMat4(m_rotation);
}
