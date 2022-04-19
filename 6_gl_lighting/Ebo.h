#pragma once
#include <glad/glad.h>
#include <iostream>

class Ebo {
public:
    //FXTODO: in the tutorial, he created 3 classes for vbo, ebo and Ebo.
    // and in Ebo class has a LinkVbo method which gets GLuint layout as an argument.
    Ebo() = delete;
    Ebo(const GLuint* indices = nullptr, GLsizeiptr indicesSize = 0);
    ~Ebo();

    void bind();
    void unbind();


private:
    GLuint m_ebo;
};
