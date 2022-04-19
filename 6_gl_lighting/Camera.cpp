#include "Camera.h"
#include <functional>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera(glm::vec3 position,
               glm::vec3 dir,
               glm::vec3 up,
               const int winWidth,
               const int winHeight,
               const float fov,
               const float near,
               const float far)
    : m_position{position}
    , m_dir{dir}
    , m_up{up}
    , m_winWidth{winWidth}
    , m_winHeight{winHeight}
    , m_fov{fov}
    , m_near{near}
    , m_far{far}
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::mvpMatrix()
{
    m_view = glm::lookAt(m_position, m_position + m_dir, m_up);
    m_proj = glm::perspective(glm::radians(m_fov), static_cast<float>(m_winWidth)/m_winHeight, m_near, m_far);
    return m_proj * m_view;
}

void Camera::handleInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        m_position += m_fSpeed * m_dir;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        m_position -= m_fSpeed * m_dir;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        m_position += m_fSpeed * glm::normalize(glm::cross(m_dir, m_up));
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        m_position -= m_fSpeed * glm::normalize(glm::cross(m_dir, m_up));
    }
//    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//        m_fRotateX += 0.5f;
//        rotate(m_fRotateX, glm::vec3(0.f, 1.0f, 0.0f));
//    }
//    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//        m_fRotateX -= 0.5f;
//        rotate(m_fRotateX, glm::vec3(0.f, 1.0f, 0.0f));
//    }
//    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//        m_fRotateY += 0.5f;
//        rotate(m_fRotateY, glm::vec3(1.f, 0.0f, 0.0f));
//    }
//    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
//        m_fRotateY -= 0.5f;
//        rotate(m_fRotateY, glm::vec3(1.f, 0.0f, 0.0f));
//    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
        m_mouseSensitivity = 0.001f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE) {
        m_mouseSensitivity = 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        m_mouseSensitivity = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
        m_mouseSensitivity = 0.01f;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        m_position = glm::vec3(0.f, 0.f, 3.f);
        m_dir = glm::vec3(0.f, 0.f, -1.f);
        m_mouseSensitivity = 0.01f;
    }

    // Handles mouse inputs
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Hides mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevents camera from jumping on the first click
        if (m_firstClick) {
            glfwSetCursorPos(window, (m_winWidth / 2), (m_winHeight / 2));
            m_firstClick = false;
        }

        // Stores the coordinates of the cursor
        double mouseX;
        double mouseY;
        // Fetches the coordinates of the cursor
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
        // and then "transforms" them into degrees
        float mouseSensitivity = 20.f;
        float rotX {mouseSensitivity * static_cast<float>(mouseY - (m_winHeight / 2)) / m_winHeight};
        float rotY {mouseSensitivity * static_cast<float>(mouseX - (m_winWidth / 2)) / m_winWidth};

        // Calculates upcoming vertical change in the Orientation
        glm::vec3 newDir = glm::rotate(m_dir, glm::radians(-rotX), glm::normalize(glm::cross(m_dir, m_up)));
        // Decides whether or not the next vertical Orientation is legal or not
        if (std::abs(glm::angle(newDir, m_up) - glm::radians(90.0f)) <= glm::radians(85.0f))
        {
            m_dir = newDir;
        }
        m_dir = glm::rotate(m_dir, glm::radians(-rotY), m_up);

        // Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
        glfwSetCursorPos(window, (m_winHeight / 2), (m_winHeight / 2));
        }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        // Unhides cursor since camera is not looking around anymore
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Makes sure the next time the camera looks around it doesn't jump
        m_firstClick = true;
    }
}

void Camera::handleMouseScroll(double xoffset, double yoffset)
{
    m_position.z += yoffset * m_mouseSensitivity;
    if (m_position.z < m_near)
        m_position.z = m_near;
    if ( m_position.z > m_far)
        m_position.z = m_far;
}
