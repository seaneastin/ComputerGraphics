#include "Camera.h"

glm::mat4 Camera::getViewMatrix()
{
    //Convert degrees to radians
    float yawRadians = glm::radians(m_yaw);
    float pitchRadians = glm::radians(m_pitch);
    //Calculate the forward vector
    glm::vec3 forward(
        cos(pitchRadians) * cos(yawRadians),
        sin(pitchRadians),
        cos(pitchRadians) * sin(yawRadians)
    );
    //Return the view matrix
    return glm::lookAt(
        m_position, //our position
        m_position + forward, //where we are looking
        glm::vec3(0.0f, 1.0f, 0.0f) //our up
    );
}

glm::mat4 Camera::getProjectionMatrix(float width, float height)
{
    return glm::perspective(
        glm::pi<float>() * 0.25f,
        width / height,
        0.1f, 1000.0f
    );
}

bool Camera::update(double deltaTime)
{
    int keyForward = GLFW_KEY_W;
    int keyBack = GLFW_KEY_S;
    int keyLeft = GLFW_KEY_A;
    int keyRight = GLFW_KEY_D;
    int keyUp = GLFW_KEY_SPACE;
    int keyDown = GLFW_KEY_LEFT_CONTROL;

    //Convert degrees to radians
    float yawRadians = glm::radians(m_yaw);
    float pitchRadians = glm::radians(m_pitch);
    //Calculate the forward vector
    glm::vec3 forward(
        cos(pitchRadians) * cos(yawRadians),
        sin(pitchRadians),
        cos(pitchRadians) * sin(yawRadians)
    );
    //Calculate the right vector
    glm::vec3 right(
        -sin(yawRadians),
        0,
        cos(yawRadians)
    );
    //Calculate the up vector
    glm::vec3 up(0.0f, 1.0f, 0.0f);

    //Find the direction
    glm::vec3 direction(

    );

    //Check input
    if (glfwGetKey(m_instance->getWindow(), keyForward)) {
        //Move forward
        m_position += forward * (float)deltaTime;
    }
    if (glfwGetKey(m_instance->getWindow(), keyBack)) {
        //Move back
        m_position -= forward * (float)deltaTime;
    }
    if (glfwGetKey(m_instance->getWindow(), keyLeft)) {
        //Move left
        m_position -= right * (float)deltaTime;
    }
    if (glfwGetKey(m_instance->getWindow(), keyRight)) {
        //Move right
        m_position += right * (float)deltaTime;
    }
    if (glfwGetKey(m_instance->getWindow(), keyUp)) {
        //Move up
        m_position += up * (float)deltaTime;
    }
    if (glfwGetKey(m_instance->getWindow(), keyDown)) {
        //Move down
        m_position -= up * (float)deltaTime;
    }

    //Get current mouse coordinates
    glfwGetCursorPos(m_instance->getWindow(), &m_currentMouseX, &m_currentMouseY);
    m_currentMouseX -= m_instance->getWidth() / 2;
    m_currentMouseY -= m_instance->getHeight() / 2;

    //Turn the camera based on the change in mouse position (delta mouse)
    if (glfwGetMouseButton(m_instance->getWindow(), GLFW_MOUSE_BUTTON_1)) {
        m_yaw += m_turnSpeed * (m_currentMouseX - m_previousMouseX);
        m_pitch -= m_turnSpeed * (m_currentMouseY - m_previousMouseY);
    }
    //Store previous mouse coordinates
    m_previousMouseX = m_currentMouseX;
    m_previousMouseY = m_currentMouseY;

    return true;
}
