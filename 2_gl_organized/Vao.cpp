#include "Vao.h"

Vao::Vao(const GLfloat *vertices, GLsizeiptr verticesSize, const GLuint *indices, GLsizeiptr indicesSize)
{
    // To draw anything, we first need a Vertex Array Object (vao).
    // and then a Vertex Buffer Object, which will read the vertices from CPU memory
    // and sends them to the GPU.
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    if (indices){
        glGenBuffers(1, &m_ebo);
    }

    // To change any attribute/value in the openGL state machine, we have to
    // bind the value we want to change to that attribute.
    // e.g. here for VertexArray.
    glBindVertexArray(m_vao);
    // or here to use our m_vbo for the buffer (there are more types of buffer,
    // so we set the type)
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    // Tell openGL how big a chunk of data to read, where to read it from and
    // how it will be used in the last argument (for performance).
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    if (indices){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);
    }

    // Vertices in graphics can have more information than just positional, such as color, normals, etc
    // Here we set the positional and how to read it.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Eventually we unbind the attributes, so we don't mess things unintentionally
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_VERTEX_ARRAY, 0);
    glBindVertexArray(0);
    // should unbind ebo after unbinding vertex array so the vao uses it! But I don't understand, wtf! Why that's not
    // a problem for  vbo?! Fuck!
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Vao::~Vao()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    //FXTODO possibly a bug and deleting a non-existant buffer!
    glDeleteBuffers(1, &m_ebo);
}

void Vao::use()
{
    // bind the VAO we created and we want to use now
    glBindVertexArray(m_vao);
}
