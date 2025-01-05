#ifndef SKYDOME_H
#define SKYDOME_H

#include "Geometry/Object.h"
#include "Renderer/Renderer.h"
#include "Renderer/Texture.h"

namespace EastWind {

class SkyDome: public Sphere
{
public:
  SkyDome(const std::string& hdri_img_path = STUDIO_HDRi_IMG);

  void SetHDRImg(const std::string& path);

  virtual void Draw(Renderer::PrimitiveType primitive_type = Renderer::PrimitiveType::Triangle) override;
private:
  Ref<Texture2D> m_texture;
};

}

#endif // !SKYDOME_H
