#ifndef KEYEVENT_H
#define KEYEVENT_H

#include "Event.h"

namespace EastWind {

class KeyEvent: public Event
{
public:
  inline int GetKeyCode() const { return m_keycode; }

  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
protected:
  KeyEvent(int keycode): m_keycode(keycode) {};
  int m_keycode;
};


class KeyPressedEvent: public KeyEvent
{
public:
  KeyPressedEvent(int keycode, int repeatCnt): KeyEvent(keycode), m_RepeatCnt(repeatCnt)
  {
  }

  inline int GetRepeatCnt() const { return m_RepeatCnt; }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent:"
       << "Keycode: " << m_keycode << "(" << m_RepeatCnt << " repeats)";

    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyPressed)
protected:
  int m_RepeatCnt;
};

class KeyReleasedEvent: public KeyEvent
{
public:
  KeyReleasedEvent(int keycode): KeyEvent(keycode)
  {
  }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent:"
       << "Keycode: " << m_keycode; 

    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyReleased)
};

class KeyTypedEvent: public KeyEvent
{
public:
  KeyTypedEvent(int keycode): KeyEvent(keycode)
  {
  }

  std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyTypedEvent:"
       << "Keycode: " << m_keycode; 

    return ss.str();
  }

  EVENT_CLASS_TYPE(KeyTyped)
};


} // EastWind


#endif // !KEYEVENT_H
