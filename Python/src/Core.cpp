#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <EastWind.h>
#include "def.h"
using namespace EastWind;

EASTWIND_BIND_FUNC_DEFINE(core, m)  {

    pybind11::class_<App>(m, "App")
        .def(pybind11::init<>())
        .def("run", &App::run)
        .def("OnEvent", &App::OnEvent)
        .def("PushLayer", &App::PushLayer)
        .def("PushOverlay", &App::PushOverlay)
        .def("GetWindow", &App::GetWindow, pybind11::return_value_policy::reference)
    ;

    // m.def("CreateApp", &CreateApp, "function that creates an App instance");

    pybind11::class_<WindowProps>(m, "WindowProps")
        .def(pybind11::init<const std::string&,uint32_t,uint32_t>())
        .def_readwrite("Title", &WindowProps::Title)
        .def_readwrite("Width", &WindowProps::Width)
        .def_readwrite("Height", &WindowProps::Height)
    ;

    pybind11::class_<Window>(m, "Window")
    ;

    pybind11::class_<CocoaWindow, Window>(m, "CocoaWindow")
        .def(pybind11::init<const WindowProps&>())
        .def("OnUpdate", &CocoaWindow::OnUpdate)
        .def("GetWidth", &CocoaWindow::GetWidth)
        .def("GetHeight", &CocoaWindow::GetHeight)
        .def("SetEventCallback", &CocoaWindow::SetEventCallback)
        .def("SetVSync", &CocoaWindow::SetVSync)
        .def("IsVSync", &CocoaWindow::IsVSync)
        .def("GetNativeWindow", &CocoaWindow::GetNativeWindow)
        .def("GetGraphicsContext", &CocoaWindow::GetGraphicsContext)
    ;


    pybind11::class_<Layer>(m, "Layer")
        .def(pybind11::init<const std::string&>())
        .def("OnAttach", &Layer::OnAttach)
        .def("OnDetach", &Layer::OnDetach)
        .def("OnUpdate", &Layer::OnUpdate)
        .def("OnEvent", &Layer::OnEvent)
        .def("GetName", &Layer::GetName)
    ;

    pybind11::class_<LayerStack>(m, "LayerStack")
        .def(pybind11::init<>())
        .def("PushLayer", &LayerStack::PushLayer)
        .def("PushOverlay", &LayerStack::PushOverlay)
        .def("PopLayer", &LayerStack::PopLayer)
        .def("PopOverlay", &LayerStack::PopOverlay)
        .def("begin", &LayerStack::begin)
        .def("end", &LayerStack::end)
    ;

    pybind11::class_<Input>(m, "Input")
        .def_static("IsKeyPressed", &Input::IsKeyPressed)
        .def_static("IsMouseButtonPressed", &Input::IsMouseButtonPressed)
        .def_static("GetMousePosition", &Input::GetMousePosition)
        .def_static("GetMouseX", &Input::GetMouseX)
        .def_static("GetMouseY", &Input::GetMouseY)
        .def_static("GetCursorRay", &Input::GetCursorRay)
    ;
}