#pragma once

#include "utils/glm_utils.h"



#include <iostream>
#include <glm/gtx/string_cast.hpp>


namespace transform3D
{
    // Translation matrix
    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {
        // TODO(student): Implement the translation matrix
        return glm::transpose(glm::mat4(1, 0, 0, translateX,
                                        0, 1, 0, translateY,
                                        0, 0, 1, translateZ,
                                        0, 0, 0, 1));
    }

    // Scaling matrix
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
        // TODO(student): Implement the scaling matrix
        return glm::transpose(glm::mat4(scaleX, 0, 0, 0,
                                        0, scaleY, 0, 0,
                                        0, 0, scaleZ, 0,
                                        0, 0, 0, 1));
    }

    // Rotation matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {
        // TODO(student): Implement the rotation matrix
        return glm::transpose(glm::mat4(cos(radians), -sin(radians), 0, 0,
                                        sin(radians), cos(radians), 0, 0,
                                        0, 0, 1, 0,
                                        0, 0, 0, 1));
    }

    // Rotation matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {
        // TODO(student): Implement the rotation matrix
        return glm::transpose(glm::mat4(cos(radians), 0, sin(radians), 0,
                                        0, 1, 0, 0,
                                        -sin(radians), 0, cos(radians), 0,
                                        0, 0, 0, 1));
    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {
        // TODO(student): Implement the rotation matrix
        return glm::transpose(glm::mat4(1, 0, 0, 0,
                                        0, cos(radians), -sin(radians), 0,
                                        0, sin(radians), cos(radians), 0,
                                        0, 0, 0, 1));
    }

    // Perspective projection matrix
    inline glm::mat4 Perspective(
        float fovy, float aspect, float zNear, float zFar)
    {
        /*std::cout << glm::to_string(glm::transpose(glm::mat4(1 / (tan((fovy / 2) * aspect)), 0, 0, 0,
            0, 1 / (tan(fovy / 2)), 0, 0,
            0, 0, (-1 * (zFar + zNear)) / (zFar - zNear), (-2 * zFar * zNear) / (zFar - zNear),
            0, 0, -1, 0))) << std::endl;
        std::cout << glm::to_string(glm::perspective(fovy, aspect, zNear, zFar)) << std::endl;*/

        return glm::perspective(fovy, aspect, zNear, zFar);

        // TODO(student): Implement the perspective projection matrix
        /*return glm::transpose(glm::mat4(1 / (tan((fovy / 2) * aspect)), 0, 0, 0,
                                        0, 1 / (tan(fovy / 2)), 0, 0,
                                        0, 0, (-1 * (zFar + zNear)) / (zFar - zNear), (-2 * zFar * zNear) / (zFar - zNear),
                                        0, 0, -1, 0));*/
    }

    // View matrix
    inline glm::mat4 View(
        const glm::vec3& position,
        const glm::vec3& forward,
        const glm::vec3& right,
        const glm::vec3& up)
    {
        // TODO(student): Implement the view matrix

        glm::mat4 mat1 = glm::transpose(glm::mat4(right.x, right.y, right.z, 0,
                                                  up.x, up.y, up.z, 0,
                                                  forward.x, forward.y, forward.z, 0,
                                                  0, 0, 0, 1));


        glm::mat4 mat2 = glm::transpose(glm::mat4(1, 0, 0, -position.x,
                                                  0, 1, 0, -position.y,
                                                  0, 0, 1, -position.z,
                                                  0, 0, 0, 1));

        glm::mat4 result = mat1 * mat2;

        return result;
    }

}   // namespace transform3D
