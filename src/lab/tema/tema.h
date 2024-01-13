#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab/tema/enemy.h"

#include "lab/lab2/transform2D.h"


namespace lab
{
    class Tema : public gfxc::SimpleScene
    {
     public:
        Tema();
        ~Tema();

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



        // Variables
        
        glm::vec3 spot_light_positions[10];
        glm::vec3 spot_light_directions[10];
        glm::vec3 spot_light_colors[10];
        float spot_light_angles[10];

        glm::vec3 point_light_positions[10];
        glm::vec3 point_light_colors[10];
        
        

        int controlled_light_source_index;
        glm::vec3 controlled_light_position;
        float angle;

        // =====================================================================================================================
        glm::vec3 playerPos;
        float playerRotY;
        float playerSpeed;
        float limbRotation1; // right arm, left leg
        float limbRotation2; // left arm, right leg
        float limbMaxRotation;
        float limbMinRotation;
        int limbDirection1; // right arm, left leg, 0 = positive rotation, 1 = negative rotation
        int limbDirection2; // left arm, right leg, 0 = positive rotation, 1 = negative rotation
        float limbRotationSpeed;
        bool isMoving;
        glm::vec3 cameraOffset;
        float curveFactor;

        glm::mat4 playerModelMat; // player model matrix used for curvature

        int groundScale;
        int lightpost_count;
        float lightpost_distance;
        glm::vec3 *lightpost_positions;
        int lightposts_per_row;
        float map_border;
        int spot_count;
        glm::vec3 *spot_positions;
        glm::vec3 *spot_directions;
        glm::vec3 *spot_colors;
        float *spot_angles;
        glm::vec2 *spot_rotations;
        float timeSinceLastRotation;
        float timeBetweenRotations;
        float spot_rotation_speed;
        float spot_rotation_max_cap;


        glm::vec3 lightpost_color;
        glm::vec3 ground_color;
        glm::vec3 player_color;

        float attackRadius;

        int enemyCount;
        Enemy *enemies;



        bool duoModeEnabled;
        transform2D::ViewportSpace viewport_space;
        glm::vec3 second_playerPos;
        float second_playerRotY;
        float second_limbRotation1; // right arm, left leg
        float second_limbRotation2; // left arm, right leg
        int second_limbDirection1; // right arm, left leg, 0 = positive rotation, 1 = negative rotation
        int second_limbDirection2; // left arm, right leg, 0 = positive rotation, 1 = negative rotation
        bool second_isMoving;
        glm::mat4 second_playerModelMat; // player model matrix used for curvature
        glm::vec3 second_player_color;

        gfxc::Camera* secondCamera;

        // =====================================================================================================================


        // Methods
        // =====================================================================================================================
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec3& color = glm::vec3(1));
        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void LoadShader(const std::string& name);
        void Attack(bool mainPlayer);
        void EnableDuoMode();
        void DrawScene(gfxc::Camera* camera, const transform2D::ViewportSpace& viewport_space, float deltaTimeSeconds);
        // =====================================================================================================================
    };
}   // namespace lab
