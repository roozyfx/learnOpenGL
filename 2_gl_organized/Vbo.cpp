#include "Vbo.h"

Vbo::Vbo(const GLfloat *vertices, GLsizeiptr verticesSize)
{
    // To draw anything, we first need a Vertex Array Object (Vbo).
    // and then a Vertex Buffer Object, which will read the vertices from CPU memory
    // and sends them to the GPU.
    glGenBuffers(1, &m_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    // Tell openGL how big a chunk of data to read, where to read it from and
    // how it will be used in the last argument (for performance).
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);
}

Vbo::~Vbo()
{
    glDeleteBuffers(1, &m_vbo);
}

void Vbo::bind()
{
    // To use vbo for the buffer (there are more types of buffer,
    // so we set the type), we have to bind it
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void Vbo::unbind()
{
    // Eventually we unbind the attributes, so we don't mess things unintentionally
    glBindBuffer(GL_VERTEX_ARRAY, 0);
}
