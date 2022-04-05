#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "Shader.h"
#include "Vao.h"

static const int WIDTH {1200};
static const int HEIGHT {1200};

const char* vertexShaderSource = "#version 460 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{"
                                 "    gl_Position = vec4(.5*aPos.x, 0.5*aPos.y, 0.5*aPos.z, 1.0);\n"
                                 "}\0";
const char* fragmentShaderSource = "#version 460 core\n"
                                   "out vec4 fragColor;\n"
                                   "void main()\n"
                                   "{"
                                   "    fragColor = vec4(0.5f, 0.2f, 0.06f, 1.0f);\n"
                                   "}\0";

int main(){
    // Use glfw to take care of creating a window
    // initialize GLFW
    glfwInit();
    // Set the openGL versions and profile for glfw. Here using openGL 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // and using the Core profile, meaning using only the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLfloat vertices[] = {
        0.f, -0.25f*float(std::sqrt(3)), 0.0f,
        1.f, -0.25f*float(std::sqrt(3)), 0.0f,
        0.5f, 0.25f*float(std::sqrt(3)), 0.0f
    };

    // another set of vertices, to use with Index Buffer Object
    GLfloat vertices2[] = {
        -1.0f, -0.25f*float(std::sqrt(3)), 0.0f, //bottom left 0
        0.f, -0.25f*float(std::sqrt(3)), 0.0f, // bottom right 1
        -0.5f, 0.25f*float(std::sqrt(3)), 0.0f, // top 2
        -.5f, -0.25f*float(std::sqrt(3)), 0.0f, // inner middle 3
        -0.75f, 0, 0.0f, // inner left 4
        -0.25f, 0, 0.0f // inner right 5
    };

    GLuint indices[] = {
        0, 3, 4,
        3, 1, 5,
        4, 5, 2
    };
    // Create the window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "FxGl", nullptr, nullptr);
    // Error check the window creation
    if (window == nullptr){
        std::cerr<<"Unable to create glfw window. Terminating!\n";
        glfwTerminate();
        return -1;
    }
    // Make the window usable by introducing it to the current gl context
    glfwMakeContextCurrent(window);

    // Load Glad to load openGL. An (old) alternative is glew.
    gladLoadGL();

    glViewport(0, 0, WIDTH, HEIGHT);

    Shader shaderProgram("shaders/default.vert", "shaders/default.frag");
    Vao vao(vertices, sizeof(vertices));

    Vao vao2(vertices2, sizeof(vertices2), indices, sizeof(indices));
    // Specify the color of the background
    glClearColor(0.07f, 0.3f, 0.6f, 1.0f);
    // Clean the back buffer and assign the new color to it
    glClear(GL_COLOR_BUFFER_BIT);
    // Swap back and front buffers
    glfwSwapBuffers(window);

    // Main loop
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.07f, 0.3f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Finally to draw, we set the program object that we want to use
        shaderProgram.use();
        vao.use();
        // and tell openGL which type of primative we want to draw and how many of it
        // is in our data
        glDrawArrays(GL_TRIANGLES, 0, 3);

        vao2.use();
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // Delete the window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW
    glfwTerminate();
    return 0;
}
