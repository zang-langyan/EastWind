#include "OpenGLRenderer.h"
#include <glad/glad.h>


namespace EastWind {

  void OpenGLRenderer::SetClearColor(const float (&color)[4]) 
  {
    glClearColor(color[0], color[1], color[2], color[3]);
  } 

  void OpenGLRenderer::Clear() 
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void OpenGLRenderer::Draw(const std::shared_ptr<BufferState>& bufferState)
  {
    glDrawElements(GL_TRIANGLES, bufferState->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
  }

}
