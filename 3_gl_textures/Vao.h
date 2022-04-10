#pragma once
#include <glad/glad.h>
#include <iostream>
#include "Vbo.h"

class Vao {
public:
    Vao();
    ~Vao();

    void bind();
    void linkVboAttrib(Vbo &vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void *offset);
    void unbind();

private:
    GLuint m_vao;
};
