#ifndef OSXINPUT_H
#define OSXINPUT_H

#include "EW_Input.h"

namespace EastWind {

class OSXInput : public Input
{
  virtual bool IsKeyPressedImpl(int keycode) override;
  virtual bool IsMouseButtonPressedImpl(int button) override;

  virtual std::pair<float, float> GetMousePositionImpl() override;
  virtual float GetMouseXImpl() override;
  virtual float GetMouseYImpl() override;
};

}

#endif // !OSXINPUT_H
