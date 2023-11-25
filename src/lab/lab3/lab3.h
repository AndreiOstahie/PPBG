#pragma once

#include "components/software_rendering_scene.h"

#include "lab/lab1/triangle_rasterizer.h"
#include "lab/lab2/transform2D.h"
#include "lab/lab3/transform3D.h"

#include "lab/lab3/depth_image.h"

namespace lab
{
    enum CULL_FACE_OPTION
    {
        BACK_FACES = 0,
        FRONT_FACES = 1,
        NO_FACES = 2,
        BOTH_FACES = 3,
        COUNT
    };

    enum TRIANGLE_FACE
    {
        BACK = 0,
        FRONT = 1,
        NONE = 10
    };

    class Lab3 : public gfxc::SoftwareRenderScene
    {
     public:
        Lab3();
        ~Lab3();

        void Initialize() override;

     private:
        void DrawCube();
        void DrawSomething();
        glm::mat4 ModelTransformation();
        void Rasterize(
            const std::vector<VertexFormat> &vertices,
            const std::vector<unsigned int> &indices,
            const glm::mat4 &transformation,
            const transform2D::ViewportSpace &viewport_space,
            CULL_FACE_OPTION face_cull_option);
        glm::vec3 ComputeClipSpacePosition(
            const glm::vec3& model_space_position,
            const glm::mat4& transformation);
        glm::vec3 ComputeScreenSpacePosition(
            const glm::vec3& clip_space_position,
            const transform2D::ViewportSpace& viewport_space);
        TRIANGLE_FACE DetermineTriangleFace(
            const glm::vec2& v1,
            const glm::vec2& v2,
            const glm::vec2& v3);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;

        DepthImage* depth_image;

        transform2D::ViewportSpace viewport_space;

        glm::vec3 camera_position;
        glm::vec3 camera_forward, camera_right, camera_up;
        CULL_FACE_OPTION cull_face_option;
    };
}   // namespace lab
