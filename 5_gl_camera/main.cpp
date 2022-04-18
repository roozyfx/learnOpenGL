#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Vao.h"
#include "Ebo.h"
#include "Vbo.h"
#include "Camera.h"

#include <iostream>
#include <cmath>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const int WIDTH {1200};
static const int HEIGHT {1200};
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

static const char* vertexShaderSource = "#version 460 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{"
                                 "    gl_Position = vec4(.5*aPos.x, 0.5*aPos.y, 0.5*aPos.z, 1.0);\n"
                                 "}\0";
static const char* fragmentShaderSource = "#version 460 core\n"
                                   "out vec4 fragColor;\n"
                                   "void main()\n"
                                   "{"
                                   "    fragColor = vec4(0.5f, 0.2f, 0.06f, 1.0f);\n"
                                   "}\0";

Camera camera(glm::vec3(0.f, 0.f, 3.f),
    glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(0.f, 1.f, 0.f) );

int main(){
    // Use glfw to take care of creating a window
    // initialize GLFW
    glfwInit();
    // Set the openGL versions and profile for glfw. Here using openGL 4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // and using the Core profile, meaning using only the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // another set of vertices, to use with Index Buffer Object
    GLfloat vertices[] = { // a prism
           // Position     ,         Color
        -0.8f, -0.8f, -0.4f,  0.9f, 0.1f, 0.1f,  0.0f, 0.0f, // bottom left 0
         0.8f, -0.8f, -0.4f,  0.2f, 0.4f, 0.26f, 1.0f, 0.0f, // bottom right 1
         0.0f,  0.8f, -0.4f,  0.1f, 0.7f, 0.2f,  1.0f, 1.0f, // top right 2
         0.0f,  0.0f,  0.4f,  0.8f, 0.6f, 0.0f,  0.0f, 1.0f // top left 3
    };

    GLuint indices[] = {
        0, 1, 2,
        2, 0, 3,
        3, 1, 2,
        3, 0, 1
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

    // load an image for the texture
    int textureWidth, textureHeight, textureChannels;
    // STB reads images from top-left to botttom-right, whereas opengl reads from bottom-left to top-right.
    // so the image read by stb is flipped in opengl. Correct it with this:
    stbi_set_flip_vertically_on_load(true);
    // and read the image
    unsigned char* textureImage = stbi_load("../res/wood0.png", &textureWidth, &textureHeight, &textureChannels, 0);

    Shader shaderProgram("../shaders/default.vert",
                         "../shaders/default.frag");

    Vao vao;
    Vbo vbo(vertices, sizeof(vertices));
    Ebo ebo(indices, sizeof(indices));
    vao.bind();
    vbo.bind();
    ebo.bind();
    vao.linkVboAttrib(vbo, 0, 3, GL_FLOAT, 8*sizeof(GL_FLOAT), (void*)0);
    vao.linkVboAttrib(vbo, 1, 3, GL_FLOAT, 8*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
    vao.linkVboAttrib(vbo, 2, 2, GL_FLOAT, 8*sizeof(GL_FLOAT), (void*)(6*sizeof(GL_FLOAT)));
    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    // Specify the color of the background
    glClearColor(0.07f, 0.3f, 0.6f, 1.0f);
    // Clean the back buffer and assign the new color to it
    glClear(GL_COLOR_BUFFER_BIT);
    // Swap back and front buffers
    glfwSwapBuffers(window);

    GLuint textureLoc = glGetUniformLocation(shaderProgram.id(), "tex0");
    shaderProgram.use();
    glUniform1i(textureLoc, 0);

    // create reference and generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    // modeify the parameters of the texture
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage);
    glGenerateMipmap(GL_TEXTURE_2D);

    // free the image
    stbi_image_free(textureImage);

    // unbind the texture to prevent mistakes
    glBindTexture(GL_TEXTURE_2D, 0);

    float rotation{0.0f};
    double prevTime{glfwGetTime()};
    constexpr double sec{1./60.};

    glfwSetScrollCallback(window, scroll_callback);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    // Main loop
    while(!glfwWindowShouldClose(window)){
        // Set background color
        glClearColor(0.07f, 0.3f, 0.6f, 1.0f);
        // Clean the back buffer and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Finally to draw, we set the program object that we want to use
        shaderProgram.use();

        vao.bind();

        // a simple timer
        double currentTime{glfwGetTime()};
        if (currentTime - prevTime >= sec) {
            rotation += 0.5f;
            prevTime = currentTime;
        }

        camera.handleInput(window);

        // rotate the model around y axis
//        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 1.0f, 0.0f));
//        camera.rotate(rotation, glm::vec3(0.f, 1.0f, 0.0f));
        /// In OpenGL, the camera is always at the center (0,0,0) and we move the world
        // move the world a little
//        view = glm::translate(view, glm::vec3(0.7f, 0.50f, -3.0f));
//        proj = glm::perspective(glm::radians(45.0f), (float)WIDTH/HEIGHT, 0.1f, 1000.0f);

        GLuint cameraMatLoc = glGetUniformLocation(shaderProgram.id(), "cameraMat");

        // Outputs the matrices into the Vertex Shader
        glUniformMatrix4fv(cameraMatLoc, 1, GL_FALSE, glm::value_ptr(camera.mvpMatrix()));

        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);
        vao.unbind();
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }

    // delete the texture to clean up
    glDeleteTextures(1, &texture);
    // Delete the window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW
    glfwTerminate();
    return 0;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.handleMouseScroll(xoffset, yoffset);
}
