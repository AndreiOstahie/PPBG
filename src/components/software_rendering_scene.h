#pragma once

#include <string>

#include "components/simple_scene.h"
#include "core/gpu/image.h"


namespace gfxc
{
    class SoftwareRenderScene : public gfxc::SimpleScene
    {
     public:
        SoftwareRenderScene();
        ~SoftwareRenderScene();

        void Init() override;

     private:
        void virtual Initialize() = 0;

        void Rasterize(std::vector<VertexFormat> vertices,
            std::vector<unsigned int> indices);
        void RasterizeTriangle(VertexFormat v1,
            VertexFormat v2, VertexFormat v3);

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

        //void OpenDialog();
        //void OnFileSelected(const std::string &fileName);

        // Processing effects
        //void SaveImage(const std::string &fileName);

     protected:
        Image *image;
        Image* depthImage;

        bool saveScreenToImage;
    };
}   // namespace m2
