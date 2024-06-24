#include "OpenGLRenderer.h"
#include "EW_Log.h"
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

  void OpenGLRenderer::Init() 
  {
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glEnable(GL_POINT_SPRITE);
    // glEnable(GL_POINT_SMOOTH);
    glPointSize(2.8);
  }

  void OpenGLRenderer::SDK_DepthTest(bool enable)
  {
    if (enable)
      glEnable(GL_DEPTH_TEST);
    else
      glDisable(GL_DEPTH_TEST);
  }

  void OpenGLRenderer::SDK_CullFace(bool enable)
  {
    if (enable)
      glEnable(GL_CULL_FACE);
    else
      glDisable(GL_CULL_FACE);
  }

  void OpenGLRenderer::SDK_SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
  {
    glViewport(x, y, width, height);
  }

  void OpenGLRenderer::Draw(const Ref<BufferState>& bufferState, const PrimitiveType& type)
  {
    /*
     * glDrawElements
     *
     * 1. mode
     * GL_POINTS
     * GL_LINE_STRIP
     * GL_LINE_LOOP
     * GL_LINES
     * GL_LINE_STRIP_ADJACENCY
     * GL_LINES_ADJACENCY
     * GL_TRIANGLE_STRIP
     * GL_TRIANGLE_FAN
     * GL_TRIANGLES
     * GL_TRIANGLE_STRIP_ADJACENCY
     * GL_TRIANGLES_ADJACENCY
     * GL_PATCHES
     *
     * 2. count
     * 
     * 3. type
     * GL_UNSIGNED_BYTE
     * GL_UNSIGNED_SHORT
     * GL_UNSIGNED_INT
     * 
     * 4.Specifies a pointer to the location where the indices are stored
     */
    switch (type)
    {
      case PrimitiveType::Dot:
        glDrawElements(GL_POINTS, bufferState->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        break;
      case PrimitiveType::Line:
        glDrawElements(GL_LINES, bufferState->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        break;
      case PrimitiveType::Triangle:
        glDrawElements(GL_TRIANGLES, bufferState->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
        break;

      default:
        EW_FATAL("Primitive Type Not Supported!!!");
    }
  }

}
