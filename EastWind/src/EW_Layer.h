#ifndef EW_LAYER_H
#define EW_LAYER_H

#include "EW_Core.h"
#include "EW_Timestep.h"
#include "Event/Event.h"

namespace EastWind {

class Layer
{
public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer();

  virtual void OnAttach() {};
  virtual void OnDetach() {};
  virtual void OnUpdate(Timestep ts) {};
  virtual void OnEvent(Event& event) {};


  inline const std::string& GetName() const {
    return m_debugName;
  }

protected:
  std::string m_debugName;
};
  
} // EastWind

#endif // !EW_LAYER_H
