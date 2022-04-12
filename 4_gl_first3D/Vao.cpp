#include "Vao.h"

Vao::Vao()
{
    // To draw anything, we first need a Vertex Array Object (vao).
    // and then a Vertex Buffer Object, which will read the vertices from CPU memory
    // and sends them to the GPU.
    glGenVertexArrays(1, &m_vao);

    // Eventually we unbind the attributes, so we don't mess things unintentionally
    glBindVertexArray(0);
}

Vao::~Vao()
{
    glDeleteVertexArrays(1, &m_vao);
}

void Vao::bind()
{
    // bind the VAO we created and we want to use now
    glBindVertexArray(m_vao);
}

void Vao::linkVboAttrib(Vbo& vbo, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
    vbo.bind();
    // Vertices in graphics can have more information than just positional, such as color, normals, etc
    // Here we set the positional and how to read it.
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    // Enable the layout
    glEnableVertexAttribArray(layout);
    vbo.unbind();
}

void Vao::unbind()
{
    glBindVertexArray(0);
}
