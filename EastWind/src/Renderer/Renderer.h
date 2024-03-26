#ifndef RENDERER_H
#define RENDERER_H

#include "BufferState.h"
#include "Shader.h"
#include "Camera.h"

namespace EastWind {

class Renderer
{
public:
  enum class API
  {
    None = 0,
    OpenGL = 1,
    Vulkan = 2,
    DX12 = 3,
    Metal = 4
  };

public:
  static void ClearColor(const float (&color)[4]);
  static void ClearBufferAndDepth();
  
  static void BeginScene(Camera& camera);
  static void EndScene();

  static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<BufferState>& bufferState);

protected:
  // SDK specific
  virtual void SetClearColor(const float (&color)[4]) = 0; 
  virtual void Clear() = 0; 

  virtual void Draw(const std::shared_ptr<BufferState>& bufferState) = 0;

public:
  inline static API GetAPI(){ return s_RendererAPI; };

  static Renderer* Create(); 
private:
  static Renderer* s_Renderer;
  static API s_RendererAPI;

  struct SceneData {
    Mat<float,4,4> VPMatrix;
  };

  static SceneData* m_SceneData;
};

}

#endif // !RENDERER_H
