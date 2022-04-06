#include "Ebo.h"

Ebo::Ebo(const GLuint *indices, GLsizeiptr indicesSize)
{
    if (indices){
        glGenBuffers(1, &m_ebo);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
    }

    // should unbind ebo after unbinding vertex array so the vao uses it! But I don't understand, wtf! Why that's not
    // a problem for  vbo?! Fuck!
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Ebo::~Ebo()
{
    glDeleteBuffers(1, &m_ebo);
}

void Ebo::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
}

void Ebo::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
