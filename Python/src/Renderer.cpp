#include <pybind11/pybind11.h>
#include <pybind11/native_enum.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <EastWind.h>
#include "def.h"
#include "PyMath.h"

using namespace EastWind;

EASTWIND_BIND_FUNC_DEFINE(renderer, m)  {

    pybind11::class_<Renderer, std::shared_ptr<Renderer>> PyRenderer(m, "Renderer");
        PyRenderer
        .def_static("ClearColor", [](const std::array<float, 4>& color) {
            float arr[4] = {color[0], color[1], color[2], color[3]};
            Renderer::ClearColor(arr);
        })
        .def_static("ClearBufferAndDepth", &Renderer::ClearBufferAndDepth)
        .def_static("Initialize", &Renderer::Initialize)
        .def_static("DepthTest", &Renderer::DepthTest)
        .def_static("CullFace", &Renderer::CullFace)
        .def_static("SetViewport", &Renderer::SetViewport)
        .def_static("BeginScene", &Renderer::BeginScene)
        .def_static("EndScene", &Renderer::EndScene)
        .def_static("Upload", [](const Ref<Shader>& shader, const std::string& name, const PyMat& mat){
            if (mat.rows() != 4 || mat.cols() != 4)
                throw std::runtime_error("Only 4x4 matrices are supported in Upload");
            Renderer::Upload(shader, name, Mat<float,4,4>(mat));
        })
        .def_static("Submit", &Renderer::Submit)
    ;

    pybind11::native_enum<Renderer::PrimitiveType>(PyRenderer, "PrimitiveType", "enum.Enum")
        .value("None", Renderer::PrimitiveType::None)
        .value("Dot", Renderer::PrimitiveType::Dot)
        .value("Line", Renderer::PrimitiveType::Line)
        .value("Triangle", Renderer::PrimitiveType::Triangle)
        .export_values()
        .finalize();

    pybind11::class_<CamPosture>(m, "CamPosture")
        .def_property("right",
            [](const CamPosture &c){ return PyVec(std::make_shared<PyVec::VecImpl<float,4>>(c.right)); },
            [](CamPosture &c, const PyVec &v){ if (v.size()!=4) throw std::runtime_error("CamPosture.right must be 4D"); c.right = Vec<float,4>(v); }
        )
        .def_property("up",
            [](const CamPosture &c){ return PyVec(std::make_shared<PyVec::VecImpl<float,4>>(c.up)); },
            [](CamPosture &c, const PyVec &v){ if (v.size()!=4) throw std::runtime_error("CamPosture.up must be 4D"); c.up = Vec<float,4>(v); }
        )
        .def_property("shoot",
            [](const CamPosture &c){ return PyVec(std::make_shared<PyVec::VecImpl<float,4>>(c.shoot)); },
            [](CamPosture &c, const PyVec &v){ if (v.size()!=4) throw std::runtime_error("CamPosture.shoot must be 4D"); c.shoot = Vec<float,4>(v); }
        )
    ;

    pybind11::class_<Camera>(m, "Camera")
        .def(pybind11::init<>())
        .def(pybind11::init([](const PyVec &position, const PyVec &up, const PyVec &direction, const PyVec &boundary){
            if (position.size()!=3 || up.size()!=3 || direction.size()!=3 || boundary.size()!=6)
                throw std::runtime_error("Camera constructor expects Vec3, Vec3, Vec3, Vec6");
            return Camera(static_cast<Vec<float,3>>(position), static_cast<Vec<float,3>>(up), static_cast<Vec<float,3>>(direction), static_cast<Vec<float,6>>(boundary));
        }))
        .def("GetPosition", [](const Camera &c){ return PyVec(std::make_shared<PyVec::VecImpl<float,4>>(c.GetPosition())); })
        .def("GetPosture", [](const Camera &c){ auto p = c.GetPosture(); return p; })
        .def("GetDirection", [](const Camera &c){ return PyVec(std::make_shared<PyVec::VecImpl<float,4>>(c.GetDirection())); })
        .def("GetUpDirection", [](const Camera &c){ return PyVec(std::make_shared<PyVec::VecImpl<float,4>>(c.GetUpDirection())); })
        .def("GetRightDirection", [](const Camera &c){ return PyVec(std::make_shared<PyVec::VecImpl<float,4>>(c.GetRightDirection())); })
        .def("GetBoundary", [](const Camera &c){ return PyVec(std::make_shared<PyVec::VecImpl<float,6>>(c.GetBoundary())); })
        .def("SetPosition", [](Camera &c, const PyVec &pos){ if (pos.size()!=4) throw std::runtime_error("Position must be 4D"); c.SetPosition(static_cast<Vec<float,4>>(pos)); })
        .def("Translate", [](Camera &c, const PyVec &dir){ if (dir.size()!=3) throw std::runtime_error("Translate expects 3D vector"); c.Translate(static_cast<Vec<float,3>>(dir)); })
        .def("WSRotateX", &Camera::WSRotateX)
        .def("WSRotateY", &Camera::WSRotateY)
        .def("WSRotateZ", &Camera::WSRotateZ)
        .def("SetWSRotationX", &Camera::SetWSRotationX)
        .def("SetWSRotationY", &Camera::SetWSRotationY)
        .def("SetWSRotationZ", &Camera::SetWSRotationZ)
        .def("ESRotateX", &Camera::ESRotateX)
        .def("ESRotateY", &Camera::ESRotateY)
        .def("ESRotateZ", &Camera::ESRotateZ)
        .def("SetESRotationX", &Camera::SetESRotationX)
        .def("SetESRotationY", &Camera::SetESRotationY)
        .def("SetESRotationZ", &Camera::SetESRotationZ)
        .def("GetViewMatrix", [](const Camera &c){ auto m = c.GetViewMatrix(); return PyMat(std::make_shared<PyMat::MatImpl<float,4,4>>(m)); })
        .def("GetViewProjectionMatrix", [](const Camera &c){ auto m = c.GetViewProjectionMatrix(); return PyMat(std::make_shared<PyMat::MatImpl<float,4,4>>(m)); })
            .def("RecalculateProjMat", [](Camera &c, const PyVec &fr){ if (fr.size()!=6) throw std::runtime_error("RecalculateProjMat expects Vec6"); c.RecalculateProjMat(static_cast<Vec<float,6>>(fr)); })
        .def("RecalculateViewMat", static_cast<void (Camera::*)()>(&Camera::RecalculateViewMat))
        .def("RecalculateViewMat4", [](Camera &c){ Vec<float,4> x,y,z,pos; c.RecalculateViewMat(x,y,z,pos); return py::make_tuple(PyVec(std::make_shared<PyVec::VecImpl<float,4>>(x)), PyVec(std::make_shared<PyVec::VecImpl<float,4>>(y)), PyVec(std::make_shared<PyVec::VecImpl<float,4>>(z)), PyVec(std::make_shared<PyVec::VecImpl<float,4>>(pos))); })
        .def("RecalculateVPMatrix", &Camera::RecalculateVPMatrix)
        .def("lookat", [](Camera &c, const PyVec &t){ if (t.size() > 4) throw std::runtime_error("lookat expects Vec4 or Vec3"); return c.lookat(static_cast<Vec<float,4>>(t)); })
        .def("SetTarget", [](Camera &c, const PyVec &t){ if (t.size() > 4) throw std::runtime_error("SetTarget expects Vec4 or Vec3"); c.SetTarget(static_cast<Vec<float,4>>(t)); })
    ;

    pybind11::class_<CameraController>(m, "CameraController")
        .def(pybind11::init<const float&>())
        .def_static("instance", &CameraController::instance, pybind11::return_value_policy::reference)
        .def("OnUpdate", &CameraController::OnUpdate)
        .def("OnEvent", [](CameraController &cc, Event* e){
            cc.OnEvent(*e);
        })
        .def("GetCamera", static_cast<Camera& (CameraController::*)()>(&CameraController::GetCamera)
                        , pybind11::return_value_policy::reference)
        .def("GetCamera", static_cast<const Camera& (CameraController::*)() const>(&CameraController::GetCamera)
                        , pybind11::return_value_policy::reference)
        .def("GetZoomLevel", &CameraController::GetZoomLevel)
        .def("SetZoomLevel", &CameraController::SetZoomLevel)
    ;

    pybind11::class_<Shader>(m, "Shader")
        .def_static("Create", static_cast<Ref<Shader> (*)(const std::string&)>(&Shader::Create))
        .def_static("Create", static_cast<
                Ref<Shader> (*)(const std::string&, const std::string&, const std::string&)
            >(&Shader::Create)
        )
    ;

    pybind11::class_<OpenGLShader, Shader>(m, "OpenGLShader")
        .def(pybind11::init<const std::string&>())
        .def(pybind11::init<const std::string&, const std::string&, const std::string&>())
        .def("Bind", &OpenGLShader::Bind)
        .def("Unbind", &OpenGLShader::Unbind)
        .def("GetName", &OpenGLShader::GetName)
        .def("reload", &OpenGLShader::reload)
        .def("need_reload", &OpenGLShader::need_reload)
        .def("UploadUniformInt", &OpenGLShader::UploadUniformInt)
        .def("UploadUniformFloat", &OpenGLShader::UploadUniformFloat)
        .def("UploadUniformFloat2", [](OpenGLShader &s, const std::string &name, const PyVec &v){ if (v.size()!=2) throw std::runtime_error("UploadUniformFloat2 expects Vec2"); s.UploadUniformFloat2(name, static_cast<Vec<float,2>>(v)); })
        .def("UploadUniformFloat3", [](OpenGLShader &s, const std::string &name, const PyVec &v){ if (v.size()!=3) throw std::runtime_error("UploadUniformFloat3 expects Vec3"); s.UploadUniformFloat3(name, static_cast<Vec<float,3>>(v)); })
        .def("UploadUniformFloat4", [](OpenGLShader &s, const std::string &name, const PyVec &v){ if (v.size()!=4) throw std::runtime_error("UploadUniformFloat4 expects Vec4"); s.UploadUniformFloat4(name, static_cast<Vec<float,4>>(v)); })
        .def("UploadUniformMat3", [](OpenGLShader &s, const std::string &name, const PyMat &m){ if (m.rows()!=3 || m.cols()!=3) throw std::runtime_error("UploadUniformMat3 expects 3x3 matrix"); s.UploadUniformMat3(name, Mat<float,3,3>(m)); })
        .def("UploadUniformMat4", [](OpenGLShader &s, const std::string &name, const PyMat &m){ if (m.rows()!=4 || m.cols()!=4) throw std::runtime_error("UploadUniformMat4 expects 4x4 matrix"); s.UploadUniformMat4(name, Mat<float,4,4>(m)); })
    ;

    pybind11::class_<GraphicsContext>(m, "GraphicsContext")
    ;  

    pybind11::class_<OpenGLContext, GraphicsContext>(m, "OpenGLContext")
        .def("Init", &OpenGLContext::Init)
        .def("MakeCurrentContext", &OpenGLContext::MakeCurrentContext)
        .def("MakeNonCurrentContext", &OpenGLContext::MakeNonCurrentContext)
        .def("SwapBuffers", &OpenGLContext::SwapBuffers)
    ;
}