#pragma once

#include <string>

#include "components/software_rendering_scene.h"
#include "core/gpu/image.h"

using namespace gfxc;


namespace lab
{
    class TriangleRasterizer
    {
     public:
        static void Rasterize(
            const VertexFormat &v1,
            const VertexFormat &v2,
            const VertexFormat &v3,
            Image *image,
            Image *depthImage = nullptr);

     private:
         static float ComputeTriangleArea(
             const glm::vec2 &v1,
             const glm::vec2 &v2,
             const glm::vec2 &v3);
         static bool CheckPointInsideTriangle(
             const glm::vec2& p,
             const VertexFormat& v1,
             const VertexFormat& v2,
             const VertexFormat& v3);
         static glm::vec3 ComputePixelColor(
             const glm::vec2& p,
             const VertexFormat& v1,
             const VertexFormat& v2,
             const VertexFormat& v3);
         static float ComputePixelDepth(
             const glm::vec2 &p,
             const VertexFormat& v1,
             const VertexFormat& v2,
             const VertexFormat& v3);
    };
}   // namespace lab
