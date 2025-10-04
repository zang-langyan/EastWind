import sys
sys.path.append("../build/Python")
import EastWind as ew
from EastWind import App, Layer
from EastWind.math import Vec, Mat, translate
import EastWind.imgui as imgui

import numpy as np

class MeshLayer(Layer):
    def __init__(self):
        super().__init__("MeshLayer")

        self.meshes = [ew.Sphere(0.1, 20, 20) for _ in range(5)]
        for i, mesh in enumerate(self.meshes):
            mesh.SetModelMatrix(translate(Vec(list(np.random.randn(3)))))
        self.cam = ew.CameraController.instance().GetCamera()
        imgui.Initialize()
        self.x = 1.0
        self.bg_color = [0.1, 0.1, 0.1, 1.0]
        # self.bg_color = imgui.ImVec4(0.1, 0.1, 0.1, 1.0)

    def __del__(self):
        imgui.Shutdown()        

    def OnUpdate(self, deltaTime):
        ew.CameraController.instance().OnUpdate(deltaTime)
        # --------- Scene Begin ---------
        # ew.Renderer.ClearColor([self.bg_color.x, self.bg_color.y, self.bg_color.z, self.bg_color.w])
        ew.Renderer.ClearColor(self.bg_color)
        ew.Renderer.ClearBufferAndDepth()
        ew.Renderer.BeginScene(self.cam)
        ew.Renderer.DepthTest(True)
        for mesh in self.meshes:
            mesh.Draw(ew.Renderer.PrimitiveType.Triangle)
        # self.mesh.Draw(ew.Renderer.PrimitiveType.Line)
        ew.Renderer.EndScene()
        # --------- Scene End ---------

        imgui.BeginFrame()
        imgui.ShowDemoWindow()
        imgui.Text(f"FPS: {1.0/float(deltaTime):.2f}")
        _, self.x = imgui.SliderFloat("X", self.x, -1.0, 1.0)
        # _, self.bg_color = imgui.ColorEdit4("BG Color", self.bg_color)
        imgui.ColorEdit4("BG Color", self.bg_color)
        imgui.Render()

    def OnEvent(self, event):
        ew.CameraController.instance().OnEvent(event)

class Sandbox(App):
    def __init__(self):
        super().__init__()
        self.mesh_layer = MeshLayer()
        self.PushLayer(self.mesh_layer)

if __name__ == "__main__":
    app = Sandbox()
    app.run()
    del app