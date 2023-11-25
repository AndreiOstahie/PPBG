#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"


namespace lab
{
    class Enemy
    {
     public:
         Enemy();
        ~Enemy();

     private:
        // Variables
         glm::vec3 position;
         glm::vec3 color;


        // Methods
         void Follow(glm::vec3 target);
       
    };
}   // namespace lab
