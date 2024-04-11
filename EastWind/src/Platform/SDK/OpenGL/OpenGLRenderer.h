#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "EW_Core.h"

#include "Renderer/Renderer.h"

namespace EastWind {

class OpenGLRenderer : public Renderer
{
public:
  virtual void SetClearColor(const float (&color)[4]) override; 
  virtual void Clear() override; 

  virtual void Init() override;

  virtual void SDK_SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

  virtual void Draw(const Ref<BufferState>& bufferState, const PrimitiveType& type) override;
};

}
#endif // !OPENGLRENDERER_H
