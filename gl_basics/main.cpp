#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

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

    // Create vertex and fragment shaders and get GLuint refrences
    // to the them.
    // Then set the source and compile them.
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    // shaders should be part of a shader program.
    // Attach shaders to the program and link it.
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //  At this point there's no need for the shader objects and the program is enough,
    // so we delete them to clean up.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // To draw anything, we first need a Vertex Array Object (vao).
    // and then a Vertex Buffer Object, which will read the vertices from CPU memory
    // and sends them to the GPU.
    GLuint VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    // To change any attribute/value in the openGL state machine, we have to
    // bind the value we want to change to that attribute.
    // e.g. here for VertexArray.
    glBindVertexArray(VAO);

    // or here to use our VBO for the buffer (there are more types of buffer,
    // so we set the type)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Tell openGL how big a chunk of data to read, where to read it from and
    // how it will be used in the last argument (for performance).
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    // Vertices in graphics can have more information than just positional, such as color, normals, etc
    // Here we set the positional and how to read it.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Eventually we unbind the attributes, so we don't mess things unintentionally
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint VAO2, VBO2, EBO;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_VERTEX_ARRAY, 0);
    glBindVertexArray(0);
    // should unbind ebo after unbinding vertex array so the vao uses it! But I don't understand, wtf! Why that's not
    // a problem for  vbo?! Fuck!
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

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
        glUseProgram(shaderProgram);
        // bind the VAO we created and we want to use now
        glBindVertexArray(VAO);
        // and tell openGL which type of primative we want to draw and how many of it
        // is in our data
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        // Take care of all GLFW events
        glfwPollEvents();
    }



    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // Delete the window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW
    glfwTerminate();
    return 0;
}
