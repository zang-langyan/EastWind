#ifndef BUFFERSTATE_H
#define BUFFERSTATE_H

#include "Buffer.h"

namespace EastWind {


class BufferState
{
public:
  virtual ~BufferState(){};

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
  virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

  virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
  virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

  static BufferState* Create();
};


}


#endif // !BUFFERSTATE_H
