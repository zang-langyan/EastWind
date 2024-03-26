#include "OpenGLBuffer.h"


namespace EastWind {

  /* *****************************************************************************
   *  Vertex Buffer
   * *****************************************************************************/
  OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
  {
    glGenBuffers(1, &m_VertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  }

  OpenGLVertexBuffer::~OpenGLVertexBuffer()
  {
    glDeleteBuffers(1, &m_VertexBufferId);
  }

  void OpenGLVertexBuffer::Bind() const 
  {
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
  }

  void OpenGLVertexBuffer::Unbind() const 
  {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  }


  /* *****************************************************************************
   *  Index Buffer
   * *****************************************************************************/
  OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_count(count)
  {
    glGenBuffers(1, &m_IndexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(uint32_t), indices, GL_STATIC_DRAW);
  }

  OpenGLIndexBuffer::~OpenGLIndexBuffer()
  {
    glDeleteBuffers(1, &m_IndexBufferId);
  }

  void OpenGLIndexBuffer::Bind() const 
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferId);
  }

  void OpenGLIndexBuffer::Unbind() const 
  {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

}
