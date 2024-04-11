#ifndef RENDERER_H
#define RENDERER_H

#include "EW_Core.h"

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

  enum class PrimitiveType
  {
    None = 0,
    Line = 1,
    Triangle = 2
  };

public:
  static void ClearColor(const float (&color)[4]);
  static void ClearBufferAndDepth();
  
  static void Initialize();

  static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

  static void BeginScene(Camera& camera);
  static void EndScene();

  static void Upload(const Ref<Shader>& shader, const std::string& name, const Mat<float,4,4>& mat);
  static void Submit(const Ref<Shader>& shader, const Ref<BufferState>& bufferState, const PrimitiveType& type = PrimitiveType::Triangle);

protected:
  // SDK specific
  virtual void SetClearColor(const float (&color)[4]) = 0; 
  virtual void Clear() = 0; 

  virtual void Init() = 0;

  virtual void SDK_SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

  virtual void Draw(const Ref<BufferState>& bufferState, const PrimitiveType & type) = 0;

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
