#ifndef BUFFERSTATE_H
#define BUFFERSTATE_H

#include "EW_Core.h"

#include "Buffer.h"

namespace EastWind {


class BufferState
{
public:
  virtual ~BufferState(){};

  virtual void Bind() const = 0;
  virtual void Unbind() const = 0;

  virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0;
  virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

  virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
  virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

  static BufferState* Create();
};


}


#endif // !BUFFERSTATE_H
