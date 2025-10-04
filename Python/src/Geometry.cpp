#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <EastWind.h>
#include "def.h"
#include "PyMath.h"

using namespace EastWind;

EASTWIND_BIND_FUNC_DEFINE(geometry, m)  {

    pybind11::class_<Vertex>(m, "Vertex")
        .def_readwrite("vid", &Vertex::vid)
        .def_property("position",
            [](const Vertex &v){ return PyVec(std::make_shared<PyVec::VecImpl<float,3>>(v.position)); },
            [](Vertex &v, const PyVec &pv){ if (pv.size()!=3) throw std::runtime_error("position must be 3D"); v.position = static_cast<Vec<float,3>>(pv); }
        )
        .def_property("vnormal",
            [](const Vertex &v){ return PyVec(std::make_shared<PyVec::VecImpl<float,3>>(v.vnormal)); },
            [](Vertex &v, const PyVec &pv){ if (pv.size()!=3) throw std::runtime_error("vnormal must be 3D"); v.vnormal = static_cast<Vec<float,3>>(pv); }
        )
        .def_property("vtangent",
            [](const Vertex &v){ return PyVec(std::make_shared<PyVec::VecImpl<float,3>>(v.vtangent)); },
            [](Vertex &v, const PyVec &pv){ if (pv.size()!=3) throw std::runtime_error("vtangent must be 3D"); v.vtangent = static_cast<Vec<float,3>>(pv); }
        )
        .def_property("vbitangent",
            [](const Vertex &v){ return PyVec(std::make_shared<PyVec::VecImpl<float,3>>(v.vbitangent)); },
            [](Vertex &v, const PyVec &pv){ if (pv.size()!=3) throw std::runtime_error("vbitangent must be 3D"); v.vbitangent = static_cast<Vec<float,3>>(pv); }
        )
        .def_property("vcolor",
            [](const Vertex &v){ return PyVec(std::make_shared<PyVec::VecImpl<float,4>>(v.vcolor)); },
            [](Vertex &v, const PyVec &pv){ if (pv.size()!=4) throw std::runtime_error("vcolor must be 4D"); v.vcolor = static_cast<Vec<float,4>>(pv); }
        )
        .def_property("vtexCoord",
            [](const Vertex &v){ return PyVec(std::make_shared<PyVec::VecImpl<float,3>>(v.vtexCoord)); },
            [](Vertex &v, const PyVec &pv){ if (pv.size()!=3) throw std::runtime_error("vtexCoord must be 3D"); v.vtexCoord = static_cast<Vec<float,3>>(pv); }
        )
        .def_readwrite("vhes", &Vertex::vhes)
    ;

    pybind11::class_<Edge>(m, "Edge")
        .def_readwrite("eid", &Edge::eid)
        .def_readwrite("ev_a", &Edge::ev_a)
        .def_readwrite("ev_b", &Edge::ev_b)
        .def_readwrite("ehe_a", &Edge::ehe_a)
        .def_readwrite("ehe_a", &Edge::ehe_a)
    ;

    pybind11::class_<Face>(m, "Face")
        .def_readwrite("fid", &Face::fid)
        .def_readwrite("indices", &Face::indices)
        .def_readwrite("fv_a", &Face::fv_a)
        .def_readwrite("fv_b", &Face::fv_b)
        .def_readwrite("fv_c", &Face::fv_c)
        .def_property("fnormal",
            [](const Face &f){ return PyVec(std::make_shared<PyVec::VecImpl<float,3>>(f.fnormal)); },
            [](Face &f, const PyVec &pv){ if (pv.size()!=3) throw std::runtime_error("fnormal must be 3D"); f.fnormal = static_cast<Vec<float,3>>(pv); }
        )
        .def_readwrite("fhe_a", &Face::fhe_a)
        .def_readwrite("fhe_b", &Face::fhe_b)
        .def_readwrite("fhe_c", &Face::fhe_c)
        .def("contain", &Face::contain)
    ;

    pybind11::class_<HalfEdge>(m, "HalfEdge")
        .def_readwrite("hid", &HalfEdge::hid)
        .def_readwrite("twin", &HalfEdge::twin)
        .def_readwrite("next", &HalfEdge::next)
        .def_readwrite("hv_a", &HalfEdge::hv_a)
        .def_readwrite("hv_b", &HalfEdge::hv_b)
        .def_readwrite("he", &HalfEdge::he)
        .def_readwrite("hf", &HalfEdge::hf)
    ;
    
    pybind11::class_<MeshData>(m, "MeshData")
        .def_readwrite("vertices", &MeshData::vertices)
        .def_readwrite("faces", &MeshData::faces)
        .def_readwrite("edges", &MeshData::edges)
        .def_readwrite("halfedges", &MeshData::halfedges)
        .def_property("central",
            [](const MeshData &md){ return PyVec(std::make_shared<PyVec::VecImpl<float,3>>(md.central)); },
            [](MeshData &md, const PyVec &pv){ if (pv.size()!=3) throw std::runtime_error("central must be 3D"); md.central = static_cast<Vec<float,3>>(pv); }
        )
    ;

    pybind11::class_<Mesh>(m, "Mesh")
        .def(pybind11::init<>())
        .def(pybind11::init<const std::string&>())
        .def("GetMeshData", &Mesh::GetMeshData)
        .def("AddVertex", [](Mesh &m, const PyVec &v){ if (v.size()!=3) throw std::runtime_error("AddVertex expects Vec3"); m.AddVertex(static_cast<Vec<float,3>>(v)); })
        .def("AddVertex", [](Mesh &m, const PyVec &v, const PyVec &n){ if (v.size()!=3 || n.size()!=3) throw std::runtime_error("AddVertex expects Vec3, Vec3"); m.AddVertex(static_cast<Vec<float,3>>(v), static_cast<Vec<float,3>>(n)); })
        .def("AddFace", &Mesh::AddFace)
        .def("Draw", [](Mesh& m, const int& primitive_type){
            Renderer::PrimitiveType pt = static_cast<Renderer::PrimitiveType>(primitive_type);
            m.Draw(pt);
        })
        .def("Draw", &Mesh::Draw)
        .def("SetModelMatrix", [](Mesh &mesh, const PyMat &m){ if (m.rows()!=4 || m.cols()!=4) throw std::runtime_error("SetModelMatrix expects 4x4 matrix"); mesh.SetModelMatrix(Mat<float,4,4>(m)); })
        .def("SetActiveShader", &Mesh::SetActiveShader)
        .def("ApplyModelMatrix", &Mesh::ApplyModelMatrix)
        .def("GetActiveShader", &Mesh::GetActiveShader)
        .def("BuildBuffer", &Mesh::BuildBuffer)
        .def("SetBufferLayout", static_cast<void (Mesh::*)(const BufferLayout&)>(&Mesh::SetBufferLayout))
        .def("SetBufferLayout", static_cast<
                void (Mesh::*)(const std::initializer_list<BufferElement>& )
            >(&Mesh::SetBufferLayout)
        )
        .def("GetBufferLayout", &Mesh::GetBufferLayout)
        .def("SetHit", &Mesh::SetHit)
        .def("IsHit", &Mesh::IsHit)
    ;

    pybind11::class_<Object, Mesh>(m, "Object")
        .def(pybind11::init<const std::string&>())
    ;

    pybind11::class_<Plane, Mesh>(m, "Plane")
        .def(pybind11::init<>())
    ;

    pybind11::class_<Cube, Mesh>(m, "Cube")
        .def(pybind11::init<>())
    ;

    pybind11::class_<Sphere, Mesh>(m, "Sphere")
        .def(pybind11::init<float, int, int>())
    ;

    pybind11::class_<Ray>(m, "Ray")
        .def(pybind11::init([](const pybind11::list& p, const pybind11::list& d){
            if (p.size() != 3 || d.size() != 3)
                throw std::runtime_error("Ray position and direction must be 3D vectors");
            return Ray(Vec3(PyVec(p)), Vec3(PyVec(d)));
        }))
        .def(pybind11::init([](const PyVec& p, const PyVec& d){
            if (p.size() != 3 || d.size() != 3)
                throw std::runtime_error("Ray position and direction must be 3D vectors");
            return Ray(static_cast<Vec<float,3>>(p), static_cast<Vec<float,3>>(d));
        }))
        .def(pybind11::init<const Ray&>())
        .def("GetOrigin", [](const Ray& self){
            auto o = self.GetOrigin();
            return PyVec(std::make_shared<PyVec::VecImpl<float,3>>(o));
        })
        .def("GetDirection", [](const Ray& self){
            auto d = self.GetDirection();
            return PyVec(std::make_shared<PyVec::VecImpl<float,3>>(d));
        })
        .def("SetOrigin", [](Ray& self, const PyVec& p){
            if (p.size() != 3)
                throw std::runtime_error("Origin must be a 3D vector");
            self.SetOrigin(static_cast<Vec<float,3>>(p));
        })
        .def("SetDirection", [](Ray& self, const PyVec& d){
            if (d.size() != 3)
                throw std::runtime_error("Direction must be a 3D vector");
            self.SetDirection(static_cast<Vec<float,3>>(d));
        })
        .def("Hit", static_cast<bool (Ray::*)(const Mesh&) const>(&Ray::Hit))
        .def("Hit", static_cast<bool (Ray::*)(const Mesh*) const>(&Ray::Hit))
        .def("Hit", [](Ray& self, const Face& face, const PyMat& modelmat){
            if (modelmat.rows() != 4 || modelmat.cols() != 4)
                throw std::runtime_error("Only 4x4 matrices are supported in Hit");
            return self.Hit(face, Mat<float,4,4>(modelmat));
        })
    ;

}