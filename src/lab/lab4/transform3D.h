#pragma once

#include "utils/glm_utils.h"

#include "lab/lab3/transform3D.h"

namespace transform3D
{
    // Translation matrix
    inline glm::mat4 Translate(const glm::vec3 &translation)
    {
        return Translate(translation.x, translation.y, translation.z);
    }

    // Scaling matrix
    inline glm::mat4 Scale(const glm::vec3 &scale)
    {
        return Scale(scale.x, scale.y, scale.z);
    }

}   // namespace transform3D
