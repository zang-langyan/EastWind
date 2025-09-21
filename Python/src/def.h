#ifndef EASTWIND_PY_DEF_H
#define EASTWIND_PY_DEF_H

#include <pybind11/pybind11.h>

#define EASTWIND_PY_MODULE_NAME EastWind

#define EASTWIND_BIND_FUNC_DECLARE(name) \
    void pybind_eastwind_##name(pybind11::module_ &)

#define EASTWIND_BIND_FUNC_DEFINE(name, m) \
    void pybind_eastwind_##name(pybind11::module_ & m)

#define EASTWIND_BIND_FUNC_CALL(name, m) \
    pybind_eastwind_##name(m)

EASTWIND_BIND_FUNC_DECLARE(example);
EASTWIND_BIND_FUNC_DECLARE(core);
EASTWIND_BIND_FUNC_DECLARE(math);
EASTWIND_BIND_FUNC_DECLARE(renderer);
EASTWIND_BIND_FUNC_DECLARE(geometry);

#endif // EASTWIND_PY_DEF_H