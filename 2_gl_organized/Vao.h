#pragma once
#include <glad/glad.h>
#include <iostream>

class Vao {
public:
    //FXTODO: in the tutorial, he created 3 classes for vbo, ebo and vao.
    // and in vao class has a LinkVbo method which gets GLuint layout as an argument.
    Vao() = delete;
    Vao(const GLfloat* vertices, GLsizeiptr verticesSize,
        const GLuint* indices = nullptr, GLsizeiptr indicesSize = 0);
    ~Vao();

    void use();

private:
    GLuint m_vao, m_vbo, m_ebo;
};
