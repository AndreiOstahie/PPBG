#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"


namespace lab
{
    class Enemy
    {
     public:
         Enemy(glm::vec3 position, glm::vec3 color);
        ~Enemy();

        // Getters
        glm::vec3 GetPosition();
        glm::vec3 GetRotation();
        glm::vec3 GetColor();
        float GetLimbRotation1();
        float GetLimbRotation2();
        float GetLimbMaxRotation();
        float GetLimbMinRotation();
        int GetLimbDirection1();
        int GetLimbDirection2();
        float GetLimbRotationSpeed();

        // Setters
        void SetPosition(glm::vec3 newPos);

        void Follow(glm::vec3 target, float deltaTime);
        void Animate(float deltaTime);

     private:
         // Variables
         glm::vec3 position;
         glm::vec3 rotation;
         glm::vec3 color;
         bool isMoving;
         float speed;
         float limbRotation1; // right arm, left leg
         float limbRotation2; // left arm, right leg
         float limbMaxRotation;
         float limbMinRotation;
         int limbDirection1; // right arm, left leg, 0 = positive rotation, 1 = negative rotation
         int limbDirection2; // left arm, right leg, 0 = positive rotation, 1 = negative rotation
         float limbRotationSpeed;


         
       
    };
}   // namespace lab
