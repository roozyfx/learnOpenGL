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

//    a Cube
    GLfloat vertices[] = {
        // Position        , Color     , texture   , normals
        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // 0 bottom face
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // 1
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // 2
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // 3

        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,// 4 top face
         0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 5
         0.5f, 0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 6
        -0.5f, 0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// 7

         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 8 east face
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 9
         0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 10
         0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // 11

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // 12 north face
         0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // 13
         0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // 14
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // 15

        -0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.f, // 16 west face
        -0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.f, // 17
        -0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.f, // 18
        -0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.f, // 19

        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 20 south face
         0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 21
         0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 22
        -0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f // 23
    };

    GLuint indices[] = {
        0, 2, 1,
        0, 3, 2,
        4, 6, 5,
        4, 7, 6,
        8, 9, 10,
        8, 10, 11,
        12, 14, 13,
        12, 15, 14,
        16, 18, 17,
        16, 19, 18,
        20, 21, 22,
        20, 22, 23
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
    vao.linkVboAttrib(vbo, 0, 3, GL_FLOAT, 11*sizeof(GL_FLOAT), (void*)0);
    vao.linkVboAttrib(vbo, 1, 3, GL_FLOAT, 11*sizeof(GL_FLOAT), (void*)(3*sizeof(GL_FLOAT)));
    vao.linkVboAttrib(vbo, 2, 2, GL_FLOAT, 11*sizeof(GL_FLOAT), (void*)(6*sizeof(GL_FLOAT)));
    vao.linkVboAttrib(vbo, 3, 3, GL_FLOAT, 11*sizeof(GL_FLOAT), (void*)(8*sizeof(GL_FLOAT)));
    vao.unbind();
    vbo.unbind();
    ebo.unbind();

    // Specify the color of the background
    glClearColor(0.07f, 0.3f, 0.6f, 1.0f);
    // Clean the back buffer and assign the new color to it
    glClear(GL_COLOR_BUFFER_BIT);
    // Swap back and front buffers
    glfwSwapBuffers(window);

    GLint textureLoc {glGetUniformLocation(shaderProgram.id(), "tex0")};
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


    // Model Matrix
    glm::mat4 model(1.0);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(2.0f, 1.0f, 3.0f));
    GLint modelLoc {glGetUniformLocation(shaderProgram.id(), "model")};


    glm::vec3 lightPos(0.9f, 0.6f, 0.0f);
    glm::vec3 lightColor(.40f, 0.3f, 1.0f);

    GLint lightPosLoc {glGetUniformLocation(shaderProgram.id(), "lightPos")};
    GLint lightColorLoc {glGetUniformLocation(shaderProgram.id(), "lightColor")};
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(lightPos));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
    shaderProgram.unbind();


    Shader lightShader("../shaders/pointLight.vert", "../shaders/pointLight.frag");
    lightShader.use();

    Vao lightVao;
    Vbo lightVbo(glm::value_ptr(lightPos), sizeof(lightPos));
    Vbo lightColorVbo(glm::value_ptr(lightColor), sizeof(lightColor));
    lightVao.bind();
    lightVbo.bind();
    lightColorVbo.bind();
    lightVao.linkVboAttrib(lightVbo, 0, 3, GL_FLOAT, 6*sizeof(GL_FLOAT), (void*)0);
    lightVao.linkVboAttrib(lightColorVbo, 1, 3, GL_FLOAT, 6*sizeof(GL_FLOAT), (void*)0);
    lightVao.unbind();
    lightVbo.unbind();
    lightColorVbo.unbind();

    lightShader.unbind();

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

        camera.handleInput(window);
        GLint cameraMatLoc {glGetUniformLocation(shaderProgram.id(), "cameraMat")};
        GLint cameraPosLoc {glGetUniformLocation(shaderProgram.id(), "cameraPos")};

        // Outputs the matrices into the Vertex Shader
        glUniformMatrix4fv(cameraMatLoc, 1, GL_FALSE, glm::value_ptr(camera.viewProjectionMat()));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(cameraPosLoc, 1, glm::value_ptr(camera.getPosition()) );

        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);
        vao.unbind();
        shaderProgram.unbind();

        lightShader.use();
        lightVao.bind();
        glDrawArrays(GL_POINTS, 0, 1);
        glPointSize( 10 );
        lightVao.unbind();
        lightShader.unbind();

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
