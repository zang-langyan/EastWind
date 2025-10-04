#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H


namespace EastWind {

class GraphicsContext
{
public:
  virtual ~GraphicsContext() = default;
  virtual void Init() = 0;
  virtual void MakeCurrentContext() = 0;
  virtual void MakeNonCurrentContext() = 0;
  virtual void SwapBuffers() = 0;
};

}

#endif // !GRAPHICSCONTEXT_H
