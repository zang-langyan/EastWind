#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <EastWind.h>
#include "def.h"

using namespace EastWind;

namespace EastWind {

// Trampoline class for Layer to allow method overrides in Python
class PyLayer : public Layer, public pybind11::trampoline_self_life_support {
public:
    using Layer::Layer;

    void OnAttach() override {
        PYBIND11_OVERLOAD(void, Layer, OnAttach, );
    }

    void OnDetach() override {
        PYBIND11_OVERLOAD(void, Layer, OnDetach, );
    }

    void OnUpdate(Timestep deltaTime) override {
        PYBIND11_OVERLOAD(void, Layer, OnUpdate, deltaTime);
    }

    void OnEvent(Event* event) override {
        PYBIND11_OVERLOAD(void, Layer, OnEvent, event);
    }
};

}