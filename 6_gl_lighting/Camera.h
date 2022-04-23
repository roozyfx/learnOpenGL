#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera(glm::vec3 position,
        glm::vec3 dir = glm::vec3(0.f, 0.f, -1.f),
        glm::vec3 up = glm::vec3(0.f, 0.f, -1.f),
        const int winWidth = 1200,
        const int winHeight = 1200,
        const float fov = 45.f,
        const float near = 0.1f,
        const float far = 1000.f
        );
    ~Camera();

    glm::mat4 viewProjectionMat();

    glm::vec3 getPosition(){return m_position;}

    void handleInput(GLFWwindow *window);
    void handleMouseScroll(double xoffset, double yoffset);

private:
    glm::vec3 m_position{0.f, 0.f, -3.f};
    glm::vec3 m_dir{0.f, 0.f, -1.f};
    glm::vec3 m_up{0.f, 1.f, 0.f};
    glm::vec3 m_right{glm::cross(m_dir, m_up)};

    glm::mat4 m_view{glm::mat4(1.f)};
    glm::mat4 m_proj{glm::mat4(1.f)};
    float m_fSpeed {0.1f};
    float m_mouseSensitivity {0.01f};
//    float m_fRotateX{0.5f};
//    float m_fRotateY{0.5f};
    int m_winWidth{1200};
    int m_winHeight{1200};
    float m_fov{45.f};
    float m_near{0.1f};
    float m_far{1000.f};
    bool m_firstClick{true};
};

