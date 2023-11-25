#pragma once

#include "utils/glm_utils.h"


namespace transform2D
{
    // Translation matrix
    inline glm::mat3 Translate(float translateX, float translateY)
    {
        // TODO(student): Implement the translation matrix
        return glm::mat3(1, 0, 0, // col 1
                         0, 1, 0, // col 2
                         translateX, translateY, 1); // col 3


        // Alternative
        /*return glm::transpose(
            glm::mat3(1, 0, tx,
                      0, 1, ty,
                      0, 0, 1)
        );*/
    }

    // Scaling matrix
    inline glm::mat3 Scale(float scaleX, float scaleY)
    {
        // TODO(student): Implement the scaling matrix
        return glm::mat3(scaleX, 0, 0, // col 1
                         0, scaleY, 0, // col 2
                         0, 0, 1); // col 3
    }

    // Rotation matrix
    inline glm::mat3 Rotate(float radians)
    {
        // TODO(student): Implement the rotation matrix
        return glm::transpose(glm::mat3(cos(radians), -sin(radians), 0,
                         sin(radians), cos(radians), 0,
                         0, 0, 1));
    }

    struct ViewportSpace
    {
        ViewportSpace() : x(0), y(0), width(1), height(1) {}
        ViewportSpace(int x, int y, int width, int height)
            : x(x), y(y), width(width), height(height) {}
        int x;
        int y;
        int width;
        int height;
    };

    struct LogicSpace
    {
        LogicSpace() : x(0), y(0), width(1), height(1) {}
        LogicSpace(float x, float y, float width, float height)
            : x(x), y(y), width(width), height(height) {}
        float x;
        float y;
        float width;
        float height;
    };

    // Viewport transformation
    inline glm::mat3 Viewport(
        const LogicSpace& logic_space,
        const ViewportSpace& viewport_space)
    {
        // TODO(student): Implement the viewport matrix. You
        // can use the translation and scaling transformations
        // described above
        return Translate(viewport_space.x, viewport_space.y) *
               Scale(viewport_space.width / logic_space.width, viewport_space.height / logic_space.height) *
               Translate(-logic_space.x, -logic_space.y);
    }

}   // namespace transform2D