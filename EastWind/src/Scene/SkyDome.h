#ifndef SKYDOME_H
#define SKYDOME_H

#include "Geometry/Object.h"
#include "Renderer/Texture.h"

namespace EastWind {

class SkyDome: public Sphere
{
public:
  SkyDome(const std::string& hdri_img_path = STUDIO_HDRi_IMG);

  void SetHDRImg(const std::string& path);

  virtual void Draw() override;
private:
  Ref<Texture2D> m_texture;
};

}

#endif // !SKYDOME_H
