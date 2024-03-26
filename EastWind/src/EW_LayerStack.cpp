#include "EW_LayerStack.h"

namespace EastWind {

  LayerStack::LayerStack()
  {
    
  }

  LayerStack::~LayerStack()
  {
    for (Layer* layer: m_layers){
      delete layer;
    }
  }

  void LayerStack::PushLayer(Layer* layer)
  {
    m_layers.emplace(m_layers.begin() + m_layerInsertIdx, layer);
    ++m_layerInsertIdx;
  }

  void LayerStack::PushOverlay(Layer* overlay)
  {
    m_layers.emplace_back(overlay);
  }

  void LayerStack::PopLayer(Layer* layer)
  {
    auto itr = std::find(m_layers.begin(), m_layers.end(), layer);

    if (itr != m_layers.end())
    {
      m_layers.erase(itr);
      --m_layerInsertIdx;
    }
  }


  void LayerStack::PopOverlay(Layer* overlay)
  {
    auto itr = std::find(m_layers.begin(), m_layers.end(), overlay);
    if (itr != m_layers.end())
    {
      m_layers.erase(itr);
    }
  }

}
