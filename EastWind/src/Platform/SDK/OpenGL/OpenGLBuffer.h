#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H

#include "Renderer/Buffer.h"
#include "Renderer/Renderer.h"

#include <glad/glad.h>
namespace EastWind{


class OpenGLVertexBuffer : public VertexBuffer
{
public:
  OpenGLVertexBuffer(float* vertices, uint32_t size);
  virtual ~OpenGLVertexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual const BufferLayout& GetLayout() const override { return m_layout; };
  virtual void SetLayout(const BufferLayout& layout) override { m_layout = layout; };
private:
  uint32_t m_VertexBufferId;
  BufferLayout m_layout;
};


class OpenGLIndexBuffer : public IndexBuffer
{
public:
  OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
  virtual ~OpenGLIndexBuffer();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual uint32_t GetCount() const override { return m_count; };

private:
  uint32_t m_IndexBufferId;
  uint32_t m_count;
};

}

#endif // !OPENGLBUFFER_H
