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


  /* *****************************************************************************
   *  Frame Buffer
   * *****************************************************************************/
  OpenGLFrameBuffer::OpenGLFrameBuffer(float width, float height) 
  {
    glGenBuffers(1, &m_FrameBufferId);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);

    glGenTextures(1, &m_texture_id);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_id, 0);

    glGenBuffers(1, &m_RenderBufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferId);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      EW_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
  }

  OpenGLFrameBuffer::~OpenGLFrameBuffer()
  {
    glDeleteBuffers(1, &m_FrameBufferId);
    glDeleteTextures(1, &m_texture_id);
    glDeleteBuffers(1, &m_RenderBufferId);
  }

  void OpenGLFrameBuffer::Bind() const
  {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
    // glBindTexture(GL_TEXTURE_2D, m_texture_id);
    // glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
  }

  void OpenGLFrameBuffer::Unbind() const
  {
    // glBindTexture(GL_TEXTURE_2D, 0);
    // glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }

  void OpenGLFrameBuffer::RescaleBuffer(float width, float height) const 
  {
    glBindTexture(GL_TEXTURE_2D, m_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_id, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferId);
  }
}
