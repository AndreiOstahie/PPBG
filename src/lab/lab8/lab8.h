#pragma once

#include "components/simple_scene.h"


namespace lab
{
    class Lab8 : public gfxc::SimpleScene
    {
     public:
        Lab8();
        ~Lab8();

        void Init() override;

     private:
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

        void LoadShader(const std::string& name,
            const std::string& VS, const std::string& FS, const std::string& GS);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture = nullptr);

     private:
        std::unordered_map<std::string, Texture2D*> mapTextures;


        float explosionAnimTime;
        int explosionAnimResetInterval;
    };
}   // namespace lab
