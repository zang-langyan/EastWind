#ifndef APPLICATIONEVENT_H
#define APPLICATIONEVENT_H

#include "Event.h"

namespace EastWind {

class WindowResizeEvent: public Event
{
public:
  WindowResizeEvent(uint32_t width, uint32_t height): m_width(width), m_height(height)
  {
  };

  inline uint32_t GetWidth()  const { return m_width; }
  inline uint32_t GetHeight() const { return m_height; }

  std::string ToString() const override
  {
    std::stringstream ss;
    ss << "WindowResizeEvent:\n" 
       << "Width: " << m_width << ", " << "Height: " << m_height; 

    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
  uint32_t m_width, m_height;
};

class WindowCloseEvent: public Event
{
public:
  WindowCloseEvent(){};

  EVENT_CLASS_TYPE(WindowClose)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};



class AppTickEvent: public Event
{
public:
  AppTickEvent(){};

  EVENT_CLASS_TYPE(AppTick)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppUpdateEvent: public Event
{
public:
  AppUpdateEvent(){};

  EVENT_CLASS_TYPE(AppUpdate)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class AppRenderEvent: public Event
{
public:
  AppRenderEvent(){};

  EVENT_CLASS_TYPE(AppRender)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

} // EastWind


#endif // !APPLICATIONEVENT_H
