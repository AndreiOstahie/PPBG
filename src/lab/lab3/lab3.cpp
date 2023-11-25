#include "lab/lab3/lab3.h"

#include <vector>
#include <iostream>

#include "components/transform.h"




#include <glm/gtx/string_cast.hpp>

using namespace std;
using namespace lab;

Lab3::Lab3()
{
    window->SetSize(1280, 720);
}

Lab3::~Lab3()
{
}

void Lab3::Initialize()
{
    Camera *camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 1, 0), glm::quatLookAt(glm::vec3 (0, 0, -1), glm::vec3 (0, 1, 0)));
    camera->Update();
    camera_position = camera->m_transform->GetWorldPosition();
    camera_forward = camera->m_transform->GetLocalOZVector();
    camera_right = camera->m_transform->GetLocalOXVector();
    camera_up = camera->m_transform->GetLocalOYVector();

    cull_face_option = CULL_FACE_OPTION::NO_FACES;

    viewport_space = { 0, 0, 1280, 720 };

    depth_image = new DepthImage();

    image->Init(1280, 720, 3 /* channels */);
    depth_image->Init(1280, 720);

    depthImage->Init(1, 1, 1);

    DrawCube();
    // DrawSomething();
}

void Lab3::DrawSomething()
{
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(5, 0, 0), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(5, 5, 0), glm::vec3(1, 0, 0)),
        
    };

    vector<unsigned int> indices
    {
        0, 1, 2,    // indices for first triangle
    };

    {
        glm::mat4 transformation = glm::mat3(1.0f);
        transformation *= transform3D::Perspective(glm::radians(60.0f), 16.0f / 9, 0.1f, 100.0f);
        transformation *= transform3D::View(camera_position, camera_forward, camera_right, camera_up);

        // std::cout << glm::to_string(transformation) << std::endl;
        Rasterize(vertices, indices, transformation, viewport_space, cull_face_option);
    }
}

void Lab3::DrawCube()
{
    vector<VertexFormat> vertices
    {
        VertexFormat(glm::vec3(-0.5, -0.5, 0.5), glm::vec3(1, 0, 0)),
        VertexFormat(glm::vec3(0.5, -0.5, 0.5), glm::vec3(0, 1, 0)),
        VertexFormat(glm::vec3(-0.5, 0.5, 0.5), glm::vec3(0, 0, 1)),
        VertexFormat(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0, 1, 1)),
        VertexFormat(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(1, 1, 0)),
        VertexFormat(glm::vec3(0.5, -0.5, -0.5), glm::vec3(1, 0, 1)),
        VertexFormat(glm::vec3(-0.5, 0.5, -0.5), glm::vec3(1, 1, 1)),
        VertexFormat(glm::vec3(0.5, 0.5, -0.5), glm::vec3(0, 0, 0)),
    };

    vector<unsigned int> indices
    {
        0, 1, 2,    // indices for first triangle
        1, 3, 2,    // indices for second triangle
        2, 3, 7,
        2, 7, 6,
        1, 7, 3,
        1, 5, 7,
        6, 7, 4,
        7, 5, 4,
        0, 4, 1,
        1, 4, 5,
        2, 6, 4,
        0, 2, 4
    };

    {
        glm::mat4 transformation = glm::mat3(1.0f);
        transformation *= transform3D::Perspective(glm::radians(60.0f), 16.0f/9, 0.1f, 100.0f);
        transformation *= transform3D::View(camera_position, camera_forward, camera_right, camera_up);
        transformation *= ModelTransformation();

        Rasterize(vertices, indices, transformation, viewport_space, cull_face_option);
    }
}

glm::mat4 Lab3::ModelTransformation()
{
    glm::mat4 transformation = glm::mat4(1);

    transformation *= transform3D::Translate(0, 1, -3);
    transformation *= transform3D::RotateOZ(glm::radians(45.0f));
    transformation *= transform3D::RotateOY(glm::radians(45.0f));
    transformation *= transform3D::RotateOX(glm::radians(45.0f));
    transformation *= transform3D::Scale(1.25f, 1.25f, 1.25f);

    return transformation;
}

void Lab3::Rasterize(
    const vector<VertexFormat> &vertices,
    const vector<unsigned int> &indices,
    const glm::mat4 &transformation,
    const transform2D::ViewportSpace &viewport_space,
    CULL_FACE_OPTION cull_face_option)
{
    if (cull_face_option == CULL_FACE_OPTION::BOTH_FACES) {
        return;
    }

    for (int i = 0; i < indices.size(); i += 3) {
        auto v1 = vertices[indices[i]];
        auto v2 = vertices[indices[i+1]];
        auto v3 = vertices[indices[i+2]];

        glm::vec3 clip_space_position1 = ComputeClipSpacePosition(v1.position, transformation);
        glm::vec3 clip_space_position2 = ComputeClipSpacePosition(v2.position, transformation);
        glm::vec3 clip_space_position3 = ComputeClipSpacePosition(v3.position, transformation);

        //std::cout << glm::to_string(v1.position) << std::endl;
        //std::cout << glm::to_string(v2.position) << std::endl;
        //std::cout << glm::to_string(v3.position) << std::endl;
        auto triangle_face = DetermineTriangleFace(clip_space_position1, clip_space_position2, clip_space_position3);

        if (triangle_face == cull_face_option) {
            continue;
        }

        v1.position = ComputeScreenSpacePosition(clip_space_position1, viewport_space);
        v2.position = ComputeScreenSpacePosition(clip_space_position2, viewport_space);
        v3.position = ComputeScreenSpacePosition(clip_space_position3, viewport_space);
        
        

        TriangleRasterizer::Rasterize(
            v1, v2, v3, image, depth_image
        );
    }
}

glm::vec3 Lab3::ComputeClipSpacePosition(
    const glm::vec3& position,
    const glm::mat4& transformation)
{
    glm::vec4 homogenous_coordinate = transformation * 
        glm::vec4(position.x, position.y, position.z, 1);

    // TODO(student): Apply the perspective division on the
    // homogeneous coordinate
    // std::cout << glm::to_string(transformation) << std::endl;

    homogenous_coordinate.x = homogenous_coordinate.x / homogenous_coordinate.w;
    homogenous_coordinate.y = homogenous_coordinate.y / homogenous_coordinate.w;
    homogenous_coordinate.z = homogenous_coordinate.z / homogenous_coordinate.w;
    glm::vec3 clip_space_pos = glm::vec3(homogenous_coordinate);

    // std::cout << glm::to_string(clip_space_pos) << std::endl;
    return clip_space_pos;
}

glm::vec3 Lab3::ComputeScreenSpacePosition(
    const glm::vec3& clip_space_position,
    const transform2D::ViewportSpace& viewport_space)
{
    transform2D::LogicSpace logic_space = { -1, -1, 2, 2 };

    glm::mat3 viewport_transformation =
        transform2D::Viewport(logic_space, viewport_space);

    glm::vec3 screen_space_position = viewport_transformation *
        glm::vec3(clip_space_position.x, clip_space_position.y, 1);
    screen_space_position.z = clip_space_position.z * 0.5 + 0.5;

    return screen_space_position;
}

TRIANGLE_FACE Lab3::DetermineTriangleFace(
    const glm::vec2& v1,
    const glm::vec2& v2,
    const glm::vec2& v3)
{
    glm::vec3 v13D = glm::vec3(v1.x, v1.y, 0);
    glm::vec3 v23D = glm::vec3(v2.x, v2.y, 0);
    glm::vec3 v33D = glm::vec3(v3.x, v3.y, 0);

    glm::vec3 cross_produt = glm::cross(v23D - v13D, v33D - v13D);

    // TODO(student): Determine and return which face of the 
    // triangle is displayed. Use the sign of the z component
    // of the cross product as follows:
    // If the sign is positive, the front face of the triangle is displayed.
    // If the sign is negative, the back face of the triangle is displayed.
    if (cross_produt.z > 0)
        return TRIANGLE_FACE::FRONT;
    else
        return TRIANGLE_FACE::BACK;
}

void Lab3::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    bool need_refresh = false;

    {
        auto camera = GetSceneCamera();
        auto cam_position = camera->m_transform->GetWorldPosition();
        auto cam_forward = camera->m_transform->GetLocalOZVector();
        auto cam_right = camera->m_transform->GetLocalOXVector();
        auto cam_up = camera->m_transform->GetLocalOYVector();

        if (cam_position != camera_position ||
            cam_forward != camera_forward ||
            cam_right != camera_right ||
            cam_up != camera_up) {

            camera_position = cam_position;
            camera_forward = cam_forward;
            camera_right = cam_right;
            camera_up = cam_up;

            need_refresh = true;
        }
    }

    if (need_refresh) {
        image->Clear(glm::vec3(0));
        depth_image->Clear();

        DrawCube();

        image->UpdateInternalData();
    }
}

void Lab3::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_F) {
        cull_face_option = (CULL_FACE_OPTION) 
            ((cull_face_option + 1) % CULL_FACE_OPTION::COUNT);

        image->Clear(glm::vec3(0));
        depth_image->Clear();

        DrawCube();

        image->UpdateInternalData();
    }
}
