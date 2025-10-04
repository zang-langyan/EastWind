#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <EastWind.h>
#include "def.h"

#include "Core.h"
using namespace EastWind;

EASTWIND_BIND_FUNC_DEFINE(core, m)  {

    pybind11::class_<App>(m, "App")
        .def(pybind11::init<>())
        .def("run", &App::run, pybind11::call_guard<pybind11::gil_scoped_release>())
        .def("OnEvent", &App::OnEvent)
        .def("PushLayer", &App::PushLayer)
        .def("PushOverlay", &App::PushOverlay)
        .def("GetWindow", &App::GetWindow, pybind11::return_value_policy::reference)
        .def_static("Get", &App::Get, pybind11::return_value_policy::reference)
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
        .def("GetNativeWindow", &CocoaWindow::GetNativeWindow, pybind11::return_value_policy::reference)
        .def("GetGraphicsContext", &CocoaWindow::GetGraphicsContext, pybind11::return_value_policy::reference)
    ;


    pybind11::class_<Layer, PyLayer, pybind11::smart_holder>(m, "Layer")
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

    pybind11::class_<Timestep>(m, "Timestep")
        .def(pybind11::init<float>())
        .def("GetSeconds", &Timestep::GetSeconds)
        .def("GetMilliSeconds", &Timestep::GetMilliSeconds)
        .def("__float__", [](const Timestep &ts) { return ts.GetSeconds(); })
    ;

    pybind11::class_<Event, pybind11::smart_holder>(m, "Event");

    pybind11::class_<WindowResizeEvent, Event, pybind11::smart_holder>(m, "WindowResizeEvent")
        .def(pybind11::init<uint32_t, uint32_t, uint32_t, uint32_t>(), pybind11::return_value_policy::reference)
        .def("GetWidth", &WindowResizeEvent::GetWidth)
        .def("GetHeight", &WindowResizeEvent::GetHeight)
        .def("GetFrameWidth", &WindowResizeEvent::GetFrameWidth)
        .def("GetFrameHeight", &WindowResizeEvent::GetFrameHeight)
        .def("ToString", &WindowResizeEvent::ToString)
    ;

    pybind11::class_<WindowCloseEvent, Event, pybind11::smart_holder>(m, "WindowCloseEvent")
        .def(pybind11::init<>(), pybind11::return_value_policy::reference)
    ;

    pybind11::class_<AppTickEvent, Event, pybind11::smart_holder>(m, "AppTickEvent")
        .def(pybind11::init<>(), pybind11::return_value_policy::reference)
    ;

    pybind11::class_<AppUpdateEvent, Event, pybind11::smart_holder>(m, "AppUpdateEvent")
        .def(pybind11::init<>(), pybind11::return_value_policy::reference)
    ;

    pybind11::class_<AppRenderEvent, Event, pybind11::smart_holder>(m, "AppRenderEvent")
        .def(pybind11::init<>(), pybind11::return_value_policy::reference)
    ;

    pybind11::class_<MouseMovedEvent, Event, pybind11::smart_holder>(m, "MouseMovedEvent")
        .def(pybind11::init<float, float>(), pybind11::return_value_policy::reference)
        .def("GetX", &MouseMovedEvent::GetX)
        .def("GetY", &MouseMovedEvent::GetY)
        .def("ToString", &MouseMovedEvent::ToString)
    ;

    pybind11::class_<MouseScrolledEvent, Event, pybind11::smart_holder>(m, "MouseScrolledEvent")
        .def(pybind11::init<float, float>(), pybind11::return_value_policy::reference)
        .def("GetXOffset", &MouseScrolledEvent::GetXOffset)
        .def("GetYOffset", &MouseScrolledEvent::GetYOffset)
        .def("ToString", &MouseScrolledEvent::ToString)
    ;

    pybind11::class_<MouseButtonPressedEvent, Event, pybind11::smart_holder>(m, "MouseButtonPressedEvent")
        .def(pybind11::init<int>(), pybind11::return_value_policy::reference)
        .def("ToString", &MouseButtonPressedEvent::ToString)
    ;

    pybind11::class_<MouseButtonReleasedEvent, Event, pybind11::smart_holder>(m, "MouseButtonReleasedEvent")
        .def(pybind11::init<int>(), pybind11::return_value_policy::reference)
        .def("ToString", &MouseButtonReleasedEvent::ToString)
    ;

    pybind11::class_<KeyEvent, Event, pybind11::smart_holder>(m, "KeyEvent")
        .def("GetKeyCode", &KeyEvent::GetKeyCode)
    ;

    pybind11::class_<KeyPressedEvent, KeyEvent, pybind11::smart_holder>(m, "KeyPressedEvent")
        .def(pybind11::init<int, int>(), pybind11::return_value_policy::reference)
        .def("GetRepeatCnt", &KeyPressedEvent::GetRepeatCnt)
        .def("ToString", &KeyPressedEvent::ToString)
    ;

    pybind11::class_<KeyReleasedEvent, KeyEvent, pybind11::smart_holder>(m, "KeyReleasedEvent")
        .def(pybind11::init<int>(), pybind11::return_value_policy::reference)
        .def("ToString", &KeyReleasedEvent::ToString)
    ;

    pybind11::class_<KeyTypedEvent, KeyEvent, pybind11::smart_holder>(m, "KeyTypedEvent")
        .def(pybind11::init<int>(), pybind11::return_value_policy::reference)
        .def("ToString", &KeyTypedEvent::ToString)
    ;
}
