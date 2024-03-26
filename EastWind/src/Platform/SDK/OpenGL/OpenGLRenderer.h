#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "Renderer/Renderer.h"

namespace EastWind {

class OpenGLRenderer : public Renderer
{
public:
  virtual void SetClearColor(const float (&color)[4]) override; 
  virtual void Clear() override; 

  virtual void Draw(const std::shared_ptr<BufferState>& bufferState) override;
};

}
#endif // !OPENGLRENDERER_H
