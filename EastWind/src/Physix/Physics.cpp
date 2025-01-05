#include "EW_pch.h"
#include "Physics.h"

#include "EW_Input.h"
#include "Physix/PhysixSDK/Bullet/Bullet.h"

namespace EastWind {

  void Physics::Update_CursorRay(std::pair<uint32_t,uint32_t> window_size, Mat4 ViewMat, Mat4 ProjMat)
  {
      auto [cursor_x, cursor_y] = EastWind::Input::GetMousePosition();
      auto [window_width, window_height] = window_size;

      Vec4 rayOrigin_NDC{
        ((cursor_x/(float)window_width) - 0.5f) * 2.f, // e.g. [0,1280] -> [-1,1]
        ((cursor_y/(float)window_height) - 0.5f) * 2.f, // e.g. [0,720] -> [-1,1]
        -1.f, // near plane
         1.f
      };

      Vec4 rayEndPoint_NDC{
        ((cursor_x/(float)window_width) - 0.5f) * 2.f,
        ((cursor_y/(float)window_height) - 0.5f) * 2.f,
         0.f, // far plane
         1.f
      };

      // Mat4 InvProjMat = ProjMat.Inverse();
      // Mat4 InvViewMat = ViewMat.Inverse();
      //
      // Vec4 rayOrigin_Camera_Space = InvProjMat * rayOrigin_NDC;
      // rayOrigin_Camera_Space /= rayOrigin_Camera_Space(3);
      // Vec4 rayOrigin_World_Space = InvViewMat * rayOrigin_Camera_Space;
      // rayOrigin_World_Space /= rayOrigin_World_Space(3);
      //
      // Vec4 rayEndPoint_Camera_Space = InvProjMat * rayEndPoint_NDC;
      // rayEndPoint_Camera_Space /= rayEndPoint_Camera_Space(3);
      // Vec4 rayEndPoint_World_Space = InvViewMat * rayEndPoint_Camera_Space;
      // rayEndPoint_World_Space /= rayEndPoint_World_Space(3);
      
      Mat4 InvViewProjMat = (ProjMat * ViewMat).Inverse(); 
      Vec4 rayOrigin_World_Space = InvViewProjMat * rayOrigin_NDC;
      rayOrigin_World_Space /= rayOrigin_World_Space(3);
      Vec4 rayEndPoint_World_Space = InvViewProjMat * rayEndPoint_NDC;
      rayEndPoint_World_Space /= rayEndPoint_World_Space(3);

      Vec3 RayOrigin(rayOrigin_World_Space);
      Vec3 RayDirection(rayEndPoint_World_Space-rayOrigin_World_Space);
      RayDirection.normalize();
      m_cursor_ray = Ray(RayOrigin, RayDirection);
  }

  Ref<Physics> Physics::Create()
  {
    switch (GetPhysicsBackEnd()) {
      case PhyBackEnd::None:
        return nullptr;
      case PhyBackEnd::Bullet:
        return std::make_shared<BulletPhyx>();
      case PhyBackEnd::PhyX:
        return nullptr;
      default:
        return nullptr;
    }
  }
}
