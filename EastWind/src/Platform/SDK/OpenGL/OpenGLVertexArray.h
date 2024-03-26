#ifndef OPENGLVERTEXARRAY_H
#define OPENGLVERTEXARRAY_H

#include "Renderer/BufferState.h"

namespace EastWind {

class OpenGLVertexArray : public BufferState
{
public:
  OpenGLVertexArray();
  virtual ~OpenGLVertexArray();

  virtual void Bind() const override;
  virtual void Unbind() const override;

  virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
  virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

  virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; };
  virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; };

private:
  uint32_t m_VertexArrayId;
  std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
  std::shared_ptr<IndexBuffer> m_indexBuffer;
};

}

#endif // !OPENGLVERTEXARRAY_H
