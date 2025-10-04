#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include <pybind11/chrono.h>
#include <EastWind.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "def.h"
#include "PyMath.h"
using namespace EastWind;


EASTWIND_BIND_FUNC_DEFINE(imgui, m)  {

    pybind11::class_<ImVec2>(m, "ImVec2")
        .def(pybind11::init<>())
        .def(pybind11::init<float, float>(), pybind11::arg("x")=0.0f, pybind11::arg("y")=0.0f)
        .def_property("x",
            [](const ImVec2 &v){ return v.x; },
            [](ImVec2 &v, float x){ v.x = x; }
        )
        .def_property("y",
            [](const ImVec2 &v){ return v.y; },
            [](ImVec2 &v, float y){ v.y = y; }
        )
        .def("__repr__", [](const ImVec2 &v) {
            return "<ImVec2 x=" + std::to_string(v.x) + " y=" + std::to_string(v.y) + ">";
        })
        .def("__eq__", [](const ImVec2 &v, const ImVec2 &o) {
            return v.x == o.x && v.y == o.y;
        })
        .def("__ne__", [](const ImVec2 &v, const ImVec2 &o) {
            return v.x != o.x || v.y != o.y;
        })
        .def("__add__", [](const ImVec2 &v, const ImVec2 &o) {
            return ImVec2(v.x + o.x, v.y + o.y);
        })
        .def("__sub__", [](const ImVec2 &v, const ImVec2 &o) {
            return ImVec2(v.x - o.x, v.y - o.y);
        })
        .def("__mul__", [](const ImVec2 &v, float s) {
            return ImVec2(v.x * s, v.y * s);
        })
        .def("__truediv__", [](const ImVec2 &v, float s) {
            return ImVec2(v.x / s, v.y / s);
        })
        .def("__getitem__", [](const ImVec2 &v, int i) {
            if (i == 0) return v.x;
            else if (i == 1) return v.y;
            throw pybind11::index_error();
        })
        .def("__setitem__", [](ImVec2 &v, int i, float val) {
            if (i == 0) v.x = val;
            else if (i == 1) v.y = val;
            else throw pybind11::index_error();
        })
    ;

    pybind11::class_<ImVec4>(m, "ImVec4")
        .def(pybind11::init<>())
        .def(pybind11::init<float, float, float, float>(),
             pybind11::arg("x")=0.0f, pybind11::arg("y")=0.0f,
             pybind11::arg("z")=0.0f, pybind11::arg("w")=0.0f)
        .def_property("x",
            [](const ImVec4 &v){ return v.x; },
            [](ImVec4 &v, float x){ v.x = x; }
        )
        .def_property("y",
            [](const ImVec4 &v){ return v.y; },
            [](ImVec4 &v, float y){ v.y = y; }
        )
        .def_property("z",
            [](const ImVec4 &v){ return v.z; },
            [](ImVec4 &v, float z){ v.z = z; }
        )
        .def_property("w",
            [](const ImVec4 &v){ return v.w; },
            [](ImVec4 &v, float w){ v.w = w; }
        )
        .def("__repr__", [](const ImVec4 &v) {
            return "<ImVec4 x=" + std::to_string(v.x) + 
                          " y=" + std::to_string(v.y) + 
                          " z=" + std::to_string(v.z) +
                          " w=" + std::to_string(v.w) + ">";
        })
        .def("__eq__", [](const ImVec4 &v, const ImVec4 &o) {
            return v.x == o.x && v.y == o.y && v.z == o.z && v.w == o.w;
        })
        .def("__ne__", [](const ImVec4 &v, const ImVec4 &o) {
            return v.x != o.x || v.y != o.y || v.z != o.z || v.w != o.w;
        })
        .def("__add__", [](const ImVec4 &v, const ImVec4 &o) {
            return ImVec4(v.x + o.x, v.y + o.y, v.z + o.z, v.w + o.w);
        })
        .def("__sub__", [](const ImVec4 &v, const ImVec4 &o) {
            return ImVec4(v.x - o.x, v.y - o.y, v.z - o.z, v.w - o.w);
        })
        .def("__mul__", [](const ImVec4 &v, float s) {
            return ImVec4(v.x * s, v.y * s, v.z * s, v.w * s);
        })
        .def("__truediv__", [](const ImVec4 &v, float s) {
            return ImVec4(v.x / s, v.y / s, v.z / s, v.w / s);
        })
        .def("__getitem__", [](const ImVec4 &v, int i) {
            if (i == 0) return v.x;
            else if (i == 1) return v.y;
            else if (i == 2) return v.z;
            else if (i == 3) return v.w;
            throw pybind11::index_error();
        })
        .def("__setitem__", [](ImVec4 &v, int i, float val) {
            if (i == 0) v.x = val;
            else if (i == 1) v.y = val;
            else if (i == 2) v.z = val;
            else if (i == 3) v.w = val;
            else throw pybind11::index_error();
        })
    ;

    m.def("Initialize", [](){
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
                                                            

        ImGuiStyle& style = ImGui::GetStyle();
        {
            // std::cout << "Viewports enabled" << std::endl;
            style.WindowRounding = 8.0f;
            style.FrameRounding = 7.0f;
            
            // style.Colors[ImGuiCol_WindowBg].w = 1.0f;
            // light style
            // style.Colors[ImGuiCol_WindowBg] = ImVec4(0.8f, 1.0f, 1.0f, 0.35f);
            // dark style
            style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.25f, 0.20f, 0.80f);
        }

        // Font Set Up
        io.Fonts->AddFontFromFileTTF(IMGUI_FONT_PATH, 20.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

        // Setup Platform/Renderer backends
        EastWind::App& app = EastWind::App::Get();

        ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow()), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
        ImGui_ImplOpenGL3_Init();
    }, "Initialize ImGui context");

    m.def("BeginFrame", [](){
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();

        static float m_time = 0.0f;
        ImGuiIO& io = ImGui::GetIO();
        float time = (float)glfwGetTime();
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.0f);
		m_time = time;

        ImGui::NewFrame();
    }, "Begin ImGui Frame");

    m.def("Render", [](){
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }, "Render ImGui Frame");

    m.def("Shutdown", [](){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }, "Shutdown ImGui and destroy context");

    m.def("ShowDemoWindow", [](bool* p_open){
        ImGui::ShowDemoWindow(p_open);
    }, pybind11::arg("p_open")=nullptr, "Show the ImGui demo window");

    m.def("Begin", [](const char* name, bool* p_open, int flags){
        return ImGui::Begin(name, p_open, flags);
    }, pybind11::arg("name"), pybind11::arg("p_open")=nullptr, pybind11::arg("flags")=0, "Begin a new ImGui window");

    m.def("End", [](){
        ImGui::End();
    }, "End the current ImGui window");

    m.def("ColorEdit4", 
        [](const char* label, ImVec4* col){
            bool res = ImGui::ColorEdit4(label, (float*)col);
            return pybind11::make_tuple(res, *col);
        }
    );
    m.def("ColorEdit4", 
        [](const char* label, pybind11::list col){
            if (pybind11::len(col) != 4 && pybind11::len(col) != 3)
                throw std::runtime_error("ColorEdit4 requires a list of 4 floats, got " + std::to_string(pybind11::len(col)));
            float vec[4];
            for (size_t i = 0; i < 4; i++) {
                vec[i] = pybind11::cast<float>(col[i]);
            }
            bool res = ImGui::ColorEdit4(label, vec);
            col[0] = vec[0];
            col[1] = vec[1];
            col[2] = vec[2];
            col[3] = vec[3];
            return pybind11::make_tuple(res, col);
        }
    );
    m.def("ColorEdit4",
        [](const char* label, PyVec col){
            if (col.size() != 4 && col.size() != 3)
                throw std::runtime_error("ColorEdit4 requires a Vec of 4 floats");
            float vec[4];
            for (size_t i = 0; i < 4; i++) {
                vec[i] = pybind11::cast<float>(col.getitem(i));
            }
            bool res = ImGui::ColorEdit4(label, vec);
            col.setitem(0, py::float_(vec[0]));
            col.setitem(1, py::float_(vec[1]));
            col.setitem(2, py::float_(vec[2]));
            col.setitem(3, py::float_(vec[3]));
            return pybind11::make_tuple(res, col);
        },
        pybind11::arg("label"), pybind11::arg("col"), "Edit 4 floats representing a color"
    );

    m.def("Text", [](const char* text){
        ImGui::Text("%s", text);
    }, pybind11::arg("text"), "Display text");
    
    m.def("TextColored", [](const ImVec4& col, const char* text){
        ImGui::TextColored((ImVec4)col, "%s", text);
    }, pybind11::arg("col"), pybind11::arg("text"), "Display colored text");

    m.def("SameLine", [](float offset_from_start_x, float spacing){
        ImGui::SameLine(offset_from_start_x, spacing);
    }, pybind11::arg("offset_from_start_x")=0.0f, pybind11::arg("spacing")=-1.0f,
    "Place the next widget on the same line as the previous widget");

    m.def("Button", [](const char* label, const ImVec2& size){
        return ImGui::Button(label, size);
    }, pybind11::arg("label"), pybind11::arg("size")=ImVec2(0,0), "Create a button");

    m.def("Checkbox", [](const char* label, bool* v){
        return ImGui::Checkbox(label, v);
    }, pybind11::arg("label"), pybind11::arg("v"), "Create a checkbox");

    m.def("SliderFloat", [](const char* label, float* v, float v_min, float v_max, const char* format, float power){
        bool changed = ImGui::SliderFloat(label, v, v_min, v_max, format, power);
        return pybind11::make_tuple(changed, *v);
    }, pybind11::arg("label"), pybind11::arg("v"), pybind11::arg("v_min"), pybind11::arg("v_max"),
       pybind11::arg("format") = "%.3f", pybind11::arg("power") = 1.0f, "Create a slider for float values and return (changed, value)");

    m.def("InputText", [](const char* label, char* buf, size_t buf_size, int flags){
        return ImGui::InputText(label, buf, buf_size, flags);
    }, pybind11::arg("label"), pybind11::arg("buf"), pybind11::arg("buf_size"), pybind11::arg("flags") = 0, "Input text field");

    m.def("InputFloat", [](const char* label, float* v, float step, float step_fast, const char* format, int flags){
        return ImGui::InputFloat(label, v, step, step_fast, format, flags);
    }, pybind11::arg("label"), pybind11::arg("v"), pybind11::arg("step") = 0.0f, pybind11::arg("step_fast") = 0.0f,
       pybind11::arg("format") = "%.3f", pybind11::arg("flags") = 0, "Input float field");

    m.def("Combo", [](const char* label, int* current_item, const std::vector<std::string>& items, int popup_max_height_in_items){
        std::vector<const char*> c_items;
        for (const auto& s : items) c_items.push_back(s.c_str());
        return ImGui::Combo(label, current_item, c_items.data(), static_cast<int>(c_items.size()), popup_max_height_in_items);
    }, pybind11::arg("label"), pybind11::arg("current_item"), pybind11::arg("items"), pybind11::arg("popup_max_height_in_items") = -1, "Combo box");

    m.def("RadioButton", [](const char* label, bool active){
        return ImGui::RadioButton(label, active);
    }, pybind11::arg("label"), pybind11::arg("active"), "Create a radio button");

    m.def("Separator", [](){
        ImGui::Separator();
    }, "Draw a horizontal separator");

    m.def("ProgressBar", [](float fraction, const ImVec2& size_arg, const char* overlay){
        ImGui::ProgressBar(fraction, size_arg, overlay);
    }, pybind11::arg("fraction"), pybind11::arg("size_arg") = ImVec2(-1,0), pybind11::arg("overlay") = nullptr, "Display a progress bar");

    m.def("InputInt", [](const char* label, int* v, int step, int step_fast, int flags){
        return ImGui::InputInt(label, v, step, step_fast, flags);
    }, pybind11::arg("label"), pybind11::arg("v"), pybind11::arg("step") = 1, pybind11::arg("step_fast") = 100, pybind11::arg("flags") = 0, "Input int field");

    m.def("DragFloat", [](const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, int flags){
        return ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, flags);
    }, pybind11::arg("label"), pybind11::arg("v"), pybind11::arg("v_speed") = 1.0f, pybind11::arg("v_min") = 0.0f, pybind11::arg("v_max") = 0.0f,
       pybind11::arg("format") = "%.3f", pybind11::arg("flags") = 0, "Draggable float value");

    m.def("TreeNode", [](const char* label){
        return ImGui::TreeNode(label);
    }, pybind11::arg("label"), "Create a tree node");

    m.def("TreePop", [](){
        ImGui::TreePop();
    }, "Pop a tree node");

}