#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include "def.h"


PYBIND11_MODULE(EASTWIND_PY_MODULE_NAME, m) {
    m.doc() = "EastWind Engine";

    pybind11::module_ math = m.def_submodule("math", "EastWind Math Module");
    EASTWIND_BIND_FUNC_CALL(example, m);
    EASTWIND_BIND_FUNC_CALL(core, m);
    EASTWIND_BIND_FUNC_CALL(math, math);
    EASTWIND_BIND_FUNC_CALL(renderer, m);
    EASTWIND_BIND_FUNC_CALL(geometry, m);

    pybind11::module_ imgui = m.def_submodule("imgui", "EastWind imgui Module");
    EASTWIND_BIND_FUNC_CALL(imgui, imgui);
}
