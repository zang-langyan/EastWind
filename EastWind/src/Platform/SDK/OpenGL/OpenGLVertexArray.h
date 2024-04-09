#ifndef OPENGLVERTEXARRAY_H
#define OPENGLVERTEXARRAY_H

#include "EW_Core.h"

#include "Renderer/BufferState.h"

namespace EastWind {

class OpenGLVertexArray : public BufferState
{
public:
  OpenGLVertexArray();
  virtual ~OpenGLVertexArray();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
  virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

  virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; };
  virtual const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; };

private:
  uint32_t m_VertexArrayId;
  std::vector<Ref<VertexBuffer>> m_vertexBuffers;
  Ref<IndexBuffer> m_indexBuffer;
};

}

#endif // !OPENGLVERTEXARRAY_H
