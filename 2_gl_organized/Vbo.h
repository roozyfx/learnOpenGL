#pragma once
#include <glad/glad.h>
#include <iostream>

class Vbo {
public:
    Vbo() = delete;
    Vbo(const GLfloat* vertices, GLsizeiptr verticesSize);
    ~Vbo();

    void bind();
    void unbind();

private:
    GLuint m_vbo;
};
