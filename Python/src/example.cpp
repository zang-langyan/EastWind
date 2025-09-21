#include "def.h"
#include <vector>
#include <pybind11/stl.h>

int add(int i, int j) {
    return i + j;
}

std::vector<int> get_vector() {
    return {1, 2, 3, 4, 5};
}

EASTWIND_BIND_FUNC_DEFINE(example, m) {
    m.def("add", &add, "A function that adds two numbers");
    m.def("get_vector", &get_vector, "A function that returns a vector of integers");
}
