#pragma once
#include <EastWind.h>


class ClearLayer: public EastWind::Layer
{
public:
  ClearLayer()
    : Layer("Begin")
  {
  }

  void OnUpdate(EastWind::Timestep ts) override
  {
    EastWind::Renderer::ClearColor({r, g, b, a});
    EastWind::Renderer::ClearBufferAndDepth();
  }

  void OnEvent(EastWind::Event& e) override
  {

  }

public:
  static float r, g, b, a;
};

float ClearLayer::r = 0.f;
float ClearLayer::g = 0.f;
float ClearLayer::b = 0.f;
float ClearLayer::a = 1.f;