
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <EastWind.h>
#include <EastWind_Math.h>
#include <EastWind_Graphics.h>
#include "def.h"
#include "PyMath.h"
using namespace EastWind;
namespace py = pybind11;


EASTWIND_BIND_FUNC_DEFINE(math, m)  {

    pybind11::class_<PyVec>(m, "Vec")
        .def(py::init<const py::list&, const std::string&>(),
             py::arg("lst"), py::arg("dtype") = "float",
             "Create a Vec from a Python list")
        .def("__getitem__", &PyVec::getitem)
        .def("__setitem__", &PyVec::setitem)
        .def("__len__", &PyVec::size)
        .def("to_list", &PyVec::to_list)
        .def("__repr__", &PyVec::repr)
         
        .def("add", &PyVec::add, "Vector addition")
        .def("sub", &PyVec::sub, "Vector subtraction")
        .def("mul", &PyVec::mul, "Vector scalar multiplication")
        .def("div", &PyVec::div, "Vector scalar division")
        .def("dot", &PyVec::dot, "Dot product")
        .def("normalized", &PyVec::normalized, "Normalized vector")
        .def("cross", &PyVec::cross, "Cross product (3D only)")
        .def("norm", &PyVec::norm, py::arg("level") = '2', "Vector norm")
        
        .def("__add__", &PyVec::add)
        .def("__sub__", &PyVec::sub)
        .def("__mul__", &PyVec::mul)
        .def("__truediv__", &PyVec::div)
        .def("__rmul__", &PyVec::mul)
        .def("__eq__", &PyVec::operator==)
        .def("__ne__", &PyVec::operator!=)
    ;


    pybind11::class_<PyMat>(m, "Mat")
        .def(py::init<const py::list&, const std::string&>(),
             py::arg("lst"), py::arg("dtype") = "float",
             "Create a Mat from a Python list of lists")
        .def("__getitem__", &PyMat::getitem)
        .def("__setitem__", &PyMat::setitem)
        .def("rows", &PyMat::rows)
        .def("cols", &PyMat::cols)
        .def("to_list", &PyMat::to_list)
        .def("__repr__", &PyMat::repr)

        .def("add", &PyMat::add, "Matrix addition")
        .def("sub", &PyMat::sub, "Matrix subtraction")
        .def("mul", &PyMat::mul, "Matrix scalar multiplication")
        .def("div", &PyMat::div, "Matrix scalar division")
        .def("matmul", &PyMat::matmul, "Matrix multiplication")
        .def("norm", &PyMat::norm, py::arg("level") = 'F', "Matrix norm")
        .def("transpose", &PyMat::transpose, "Transpose of the matrix")
        .def("inverse", &PyMat::inverse, "Inverse of the matrix")

        .def("__add__", &PyMat::add)
        .def("__sub__", &PyMat::sub)
        .def("__mul__", &PyMat::mul)
        .def("__mul__", &PyMat::matmul)
        .def("__truediv__", &PyMat::div)
        .def("__matmul__", &PyMat::matmul)
        .def("__rmul__", &PyMat::mul)
        .def("__eq__", &PyMat::operator==)
        .def("__ne__", &PyMat::operator!=)
    ;

    m.def("degree2radian", static_cast<float  (*)(const float&) >(&EastWind::degree2radian));
    m.def("degree2radian", static_cast<double (*)(const double&)>(&EastWind::degree2radian));
    m.def("radian2degree", static_cast<float  (*)(const float&) >(&EastWind::radian2degree));
    m.def("radian2degree", static_cast<double (*)(const double&)>(&EastWind::radian2degree));
    // Expose length functions accepting PyVec and converting to native Vec<T,n>
    m.def("len", [](const PyVec &v)->float {
        return EastWind::len(static_cast<Vec<float,2>>(v));
    });
    m.def("len", [](const PyVec &v)->double {
        return EastWind::len(static_cast<Vec<double,2>>(v));
    });
    m.def("len", [](const PyVec &v)->float {
        return EastWind::len(static_cast<Vec<float,3>>(v));
    });
    m.def("len", [](const PyVec &v)->double {
        return EastWind::len(static_cast<Vec<double,3>>(v));
    });

    pybind11::class_<CamFrustum>(m, "CamFrustum")
        .def(pybind11::init([](const py::list& params) {
            PyVec v(params, "float");
            return CamFrustum(static_cast<Vec<float,6>>(v));
        }), py::arg("params"))
        .def(pybind11::init([](){
            return CamFrustum(Vec<float,6>({-1.f, 1.f, -1.f, 1.f, 0.1f, 100.f}));
        }))
        .def_readwrite("l", &CamFrustum::l)
        .def_readwrite("r", &CamFrustum::r)
        .def_readwrite("t", &CamFrustum::t)
        .def_readwrite("b", &CamFrustum::b)
        .def_readwrite("n", &CamFrustum::n)
        .def_readwrite("f", &CamFrustum::f)
        .def("__repr__", [](const CamFrustum& self) {
            return "<CamFrustum l=" + std::to_string(self.l) +
               " r=" + std::to_string(self.r) +
               " t=" + std::to_string(self.t) +
               " b=" + std::to_string(self.b) +
               " n=" + std::to_string(self.n) +
               " f=" + std::to_string(self.f) + ">";
        })
        ;

    m.def("translate", [](PyVec v) -> PyMat {
        if (v.size() == 3) {
            auto vec = static_cast<Vec<float,3>>(v);
            auto mat = EastWind::translate(vec);
            return PyMat(std::make_shared<PyMat::MatImpl<float,4,4>>(mat));
        } else {
            throw std::runtime_error("Translation vector must be 3D");
        }
        
    });
    m.def("scale", [](PyVec v) -> PyMat {
        if (v.size() == 3) {
            auto vec = static_cast<Vec<float,3>>(v);
            auto mat = EastWind::scale(vec);
            return PyMat(std::make_shared<PyMat::MatImpl<float,4,4>>(mat));
        } else {
            throw std::runtime_error("Scale vector must be 3D");
        }
    });

    m.def("rotateX", [](float angle) -> PyMat {
        auto mat = EastWind::rotateX(angle);
        return PyMat(std::make_shared<PyMat::MatImpl<float,4,4>>(mat));
    });

    m.def("rotateY", [](float angle) -> PyMat {
        auto mat = EastWind::rotateY(angle);
        return PyMat(std::make_shared<PyMat::MatImpl<float,4,4>>(mat));
    });

    m.def("rotateZ", [](float angle) -> PyMat {
        auto mat = EastWind::rotateZ(angle);
        return PyMat(std::make_shared<PyMat::MatImpl<float,4,4>>(mat));
    });

    m.def("cameraTranslate", [](PyVec v) -> PyMat {
        if (v.size() == 3) {
            auto vec = static_cast<Vec<float,3>>(v);
            auto mat = EastWind::cameraTranslate(vec);
            return PyMat(std::make_shared<PyMat::MatImpl<float,4,4>>(mat));
        } else {
            throw std::runtime_error("Camera translation vector must be 3D");
        }
    });

    m.def("cameraRotate", [](const PyVec &right, const PyVec &up, const PyVec &direction) -> PyMat {
        if (right.size() != 4 || up.size() != 4 || direction.size() != 4)
            throw std::runtime_error("cameraRotate expects three 4D vectors");
        auto mat = EastWind::cameraRotate(static_cast<Vec<float,4>>(right), static_cast<Vec<float,4>>(up), static_cast<Vec<float,4>>(direction));
        return PyMat(std::make_shared<PyMat::MatImpl<float,4,4>>(mat));
    });

    m.def("cameraPerspective", [](const CamFrustum &prop) -> PyMat {
        auto mat = EastWind::cameraPerspective(prop);
        return PyMat(std::make_shared<PyMat::MatImpl<float,4,4>>(mat));
    });

    m.def("cameraOrtho", [](const CamFrustum &prop) -> PyMat {
        auto mat = EastWind::cameraOrtho(prop);
        return PyMat(std::make_shared<PyMat::MatImpl<float,4,4>>(mat));
    });
    
}