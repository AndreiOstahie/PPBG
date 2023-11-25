#pragma once

#include <vector>

#include "components/simple_scene.h"

#include "lab/lab2/transform2D.h"

namespace lab
{
    class Lab4 : public gfxc::SimpleScene
    {
     public:
        Lab4();
        ~Lab4();

        void Init() override;

     private:
        void CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);
        void DrawObjects(gfxc::Camera *camera, const transform2D::ViewportSpace &viewport_space);

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix,
            const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

     protected:
        transform2D::ViewportSpace viewport_space;
        glm::vec3 color;
        GLenum cullFace;

        // Task 4
     protected:
         float minPos;
         float maxPos;
         float currPos;
         float maxScale;
         float minScale;
         float currScale;
         float currRotation;
         float rotationSpeed;
         bool scaleUp;
         bool moveUp;

         // Task 5
     protected:
         float cubePosX;
         float cubePosY;
         float cubePosZ;
         glm::vec3 cubeColor;

         // Bonus
     protected:
         bool bonus;
         float sunMinX;
         float sunMaxX;
         float sunCurrX;
         float sunSpeed;
         bool sunMoveRight;
         float earthCurrRotation;
         float earthRotationSpeed;
         float moonCurrRotation;
         float moonRotationSpeed;
         float sunEarthDistance;
         float earthMoonDistance;


    private:
        void DrawPlanetSystem(gfxc::Camera* camera, const transform2D::ViewportSpace& viewport_space);
    };
}   // namespace lab
