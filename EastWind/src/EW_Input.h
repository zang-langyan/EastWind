#ifndef EW_INPUT_H
#define EW_INPUT_H


namespace EastWind {

class Input
{
public:
  inline static bool IsKeyPressed(int keycode) { return s_InputInstance->IsKeyPressedImpl(keycode); }
  inline static bool IsMouseButtonPressed(int button) { return s_InputInstance->IsMouseButtonPressedImpl(button); }

  inline static std::pair<float, float> GetMousePosition() { return s_InputInstance->GetMousePositionImpl(); }
  inline static float GetMouseX() { return s_InputInstance->GetMouseXImpl(); }
  inline static float GetMouseY() { return s_InputInstance->GetMouseYImpl(); }

protected:
  virtual bool IsKeyPressedImpl(int keycode) = 0;
  virtual bool IsMouseButtonPressedImpl(int button) = 0;

  virtual std::pair<float, float> GetMousePositionImpl() = 0;
  virtual float GetMouseXImpl() = 0;
  virtual float GetMouseYImpl() = 0;
private:
  static Input* s_InputInstance;
};

}

#endif // !EW_INPUT_H
