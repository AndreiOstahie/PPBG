#include "lab/lab1/lab1.h"

#include <vector>

using namespace std;
using namespace lab;

Lab1::Lab1()
{
    window->SetSize(1280, 720);
}


Lab1::~Lab1()
{
}

void Lab1::Initialize()
{
    image->Init(1280, 720, 3 /* channels */);
    depthImage->Init(1280, 720, 1 /* channels */, glm::vec3 (1) /* initial value */);

    bool bonus = true;

    if (!bonus)
    {
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(glm::vec3(290, 90,  0.5), glm::vec3(1, 0, 0)),
                VertexFormat(glm::vec3(1099, 450,  0.5), glm::vec3(0, 1, 0)),
                VertexFormat(glm::vec3(650, 719,  0.5), glm::vec3(0, 0, 1)),

                VertexFormat(glm::vec3(200, 450,  1), glm::vec3(0, 1, 1)),
                VertexFormat(glm::vec3(830, 719,  0), glm::vec3(1, 1, 0)),
                VertexFormat(glm::vec3(1099, 0,  0), glm::vec3(1, 0, 1))
            };

            vector<unsigned int> indices
            {
                0, 1, 2,    // indices for first triangle
                3, 4, 5,    // indices for second triangle
            };

            Rasterize(vertices, indices);
        }
    }
    else
    {
        vector<VertexFormat> vertices;
        vector<unsigned int> indices;

        int count = 6;
        float a = 640;
        float b = 360;
        float r = 100;
        float alpha_base = 360 / count;
        float x, y;

        // Center vertex
        vertices.push_back(VertexFormat(glm::vec3(a, b, 0.5f), glm::vec3(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f)));

        for (int i = 0; i <= count; i++)
        {
            x = a + r * cos(glm::radians(alpha_base * i));
            y = b + r * sin(glm::radians(alpha_base * i));

            vertices.push_back(VertexFormat(glm::vec3(x, y, 0.5f), glm::vec3(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f)));
        }

        for (int i = 1; i <= count; i++) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }

        indices.push_back(0);
        indices.push_back(count);
        indices.push_back(1);


        /*indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);

        indices.push_back(0);
        indices.push_back(2);
        indices.push_back(3);*/



        Rasterize(vertices, indices);
    }
}

void Lab1::Rasterize(
    const vector<VertexFormat> &vertices,
    const vector<unsigned int> &indices)
{
    for (int i = 0; i < indices.size(); i += 3) {
        auto v1 = vertices[indices[i]];
        auto v2 = vertices[indices[i+1]];
        auto v3 = vertices[indices[i+2]];

        TriangleRasterizer::Rasterize(
            v1, v2, v3, image, depthImage
        );
    }
}
