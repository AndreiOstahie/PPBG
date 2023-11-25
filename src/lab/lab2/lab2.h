#pragma once

#include "components/software_rendering_scene.h"

#include "lab/lab1/triangle_rasterizer.h"
#include "lab/lab2/transform2D.h"

namespace lab
{
    class Lab2 : public gfxc::SoftwareRenderScene
    {
     public:
        Lab2();
        ~Lab2();

        void Initialize() override;

     private:
        void DrawShapes();
        void Rasterize(
            const std::vector<VertexFormat> &vertices,
            const std::vector<unsigned int> &indices,
            const glm::mat3 transformation);
        void OnInputUpdate(float deltaTime, int mods) override;

        transform2D::LogicSpace logic_space;
        transform2D::ViewportSpace viewport_space;
    };
}   // namespace lab
