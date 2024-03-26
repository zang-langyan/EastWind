#ifndef EW_LAYERSTACK_H
#define EW_LAYERSTACK_H

#include "EW_Core.h"
#include "EW_Layer.h"


namespace EastWind {

class LayerStack
{
public:
  LayerStack();
  ~LayerStack();

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* overlay);
  void PopLayer(Layer* layer);
  void PopOverlay(Layer* overlay);

  std::vector<Layer*>::iterator begin() {
    return m_layers.begin();
  }
  std::vector<Layer*>::iterator end() {
    return m_layers.end();
  }
private:
  std::vector<Layer*> m_layers;
  unsigned m_layerInsertIdx = 0;
};

}


#endif // EW_LAYERSTACK_H
