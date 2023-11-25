#pragma once

#include <vector>

#include "components/simple_scene.h"

#include "lab/lab2/transform2D.h"
#include "lab/lab4/transform3D.h"

namespace lab
{
    class Lab5 : public gfxc::SimpleScene
    {
     public:
        Lab5();
        ~Lab5();

        void Init() override;

        Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);

     private:
        void CreateShader(const char *name, const char *vertex_shader_path, const char *fragment_shader_path);

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &model,
            const gfxc::Camera *camera, const transform2D::ViewportSpace &viewport_space);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        const char *GetShaderContent(const char *shader_file_path);
        void CheckShaderCompilationError(unsigned int shader_id);
        void CheckShadersLinkingError(unsigned int program_id);

     protected:
        transform2D::ViewportSpace viewport_space;
    };
}   // namespace m1
