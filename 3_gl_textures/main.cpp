#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Vao.h"
#include "Ebo.h"
#include "Vbo.h"

#include <iostream>
#include <cmath>
#include <stb/stb_image.h>


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

    // another set of vertices, to use with Index Buffer Object
    GLfloat vertices[] = {
           // Position    ,         Color
        -0.8f, -0.8f, 0.0f,  0.9f, 0.1f, 0.1f,  0.0f, 0.0f, // bottom left 0
         0.8f, -0.8f, 0.0f,  0.2f, 0.4f, 0.26f, 1.0f, 0.0f, // bottom right 1
         0.8f,  0.8f, 0.0f,  0.1f, 0.7f, 0.2f,  1.0f, 1.0f, // top right 2
        -0.8f,  0.8f, 0.0f,  0.8f, 0.6f, 0.0f,  0.0f, 1.0f // top left 3
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
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

    // Get location of uniforms:
    GLuint scaleLoc = glGetUniformLocation(shaderProgram.id(), "scale");
    GLuint tranformLoc = glGetUniformLocation(shaderProgram.id(), "transform");
    const GLfloat transform[] = {0.0f, .0f, 0.0f};
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

    // Main loop
    while(!glfwWindowShouldClose(window)){
        glClearColor(0.07f, 0.3f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Finally to draw, we set the program object that we want to use
        shaderProgram.use();

        vao.bind();
        glUniform1f(scaleLoc, 1.0f);
        glUniform3fv(tranformLoc, 1, &transform[0]);
        glBindTexture(GL_TEXTURE_2D, texture);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
