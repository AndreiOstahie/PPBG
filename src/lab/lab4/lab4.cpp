#include "lab/lab4/lab4.h"

#include <vector>
#include <iostream>

#include "components/transform.h"
#include "core/engine.h"
#include "utils/gl_utils.h"

#include "lab/lab4/transform3D.h"

using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab4::Lab4()
{
}


Lab4::~Lab4()
{
}


void Lab4::Init()
{
    // Bonus
    bonus = true;
    sunMinX = -5.0f;
    sunMaxX = 5.0f;
    sunCurrX = 0.0f;
    sunSpeed = 2.0f;
    sunMoveRight = true;
    earthCurrRotation = 0.0f;
    earthRotationSpeed = 35.0f;
    moonCurrRotation = 0.0f;
    moonRotationSpeed = 75.0f;
    sunEarthDistance = 5.0f;
    earthMoonDistance = 2.0f;

    // Task 4
    minPos = 0.0f;
    maxPos = 3.0f;
    currPos = 0.0f;
    minScale = 0.5f;
    maxScale = 2.0f;
    currScale = 1.0f;
    scaleUp = true;
    moveUp = true;
    currRotation = 0.0f;
    rotationSpeed = 35.0f;
    

    // Task 5
    cubePosX = 0.0f;
    cubePosY = 0.0f;
    cubePosZ = 0.0f;
    cubeColor = glm::vec3(1, 0.5, 0);


    color = glm::vec3(0);

    viewport_space = {0, 0, 1920, 1080};

    cullFace = GL_BACK;

    // Create a mesh box using custom data
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

        // Create the mesh from the data
        CreateMesh("cube", vertices, indices);
    }


    // Another cube
    // Create a mesh box using custom data
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-0.5, -0.5, 0.5), cubeColor),
            VertexFormat(glm::vec3(0.5, -0.5, 0.5), cubeColor),
            VertexFormat(glm::vec3(-0.5, 0.5, 0.5), cubeColor),
            VertexFormat(glm::vec3(0.5, 0.5, 0.5), cubeColor),
            VertexFormat(glm::vec3(-0.5, -0.5, -0.5), cubeColor),
            VertexFormat(glm::vec3(0.5, -0.5, -0.5), cubeColor),
            VertexFormat(glm::vec3(-0.5, 0.5, -0.5), cubeColor),
            VertexFormat(glm::vec3(0.5, 0.5, -0.5), cubeColor),
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

        // Create the mesh from the data
        CreateMesh("cube2", vertices, indices);
    }
}


void Lab4::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices)
{
    unsigned int VAO;
    // TODO(student): Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    // TODO(student): Create the VBO and bind it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // TODO(student): Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO;
    // TODO(student): Create the IBO and bind it
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // TODO(student): Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data. It will be learned later, when GLSL shaders will be
    // introduced. For the moment, just think that each property value from
    // our vertex format needs to be sent to a certain channel, in order to
    // know how to receive it in the GLSL vertex shader.

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // TODO(student): Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}


void Lab4::FrameStart()
{

}


void Lab4::Update(float deltaTimeSeconds)
{
    if (!bonus)
    {
        // Task 4
        if (currPos >= maxPos) {
            moveUp = false;
        }
        if (currPos <= minPos) {
            moveUp = true;
        }
        if (moveUp) {
            currPos += deltaTimeSeconds;
        }
        else {
            currPos -= deltaTimeSeconds;
        }

        if (currScale >= maxScale) {
            scaleUp = false;
        }
        if (currScale <= minScale) {
            scaleUp = true;
        }
        if (scaleUp) {
            currScale += deltaTimeSeconds;
        }
        else {
            currScale -= deltaTimeSeconds;
        }

        currRotation += rotationSpeed * deltaTimeSeconds;


        glm::ivec2 resolution = window->GetResolution();

        // TODO(student): Set the screen cleaning color. Use the 'color' attribute.
        glClearColor(color.r, color.g, color.b, 1);

        // Clears the color buffer (using the previously set color) and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static auto camera = new gfxc::Camera();

        // TODO(student): Draw the objects 4 times in different viewports.
        // Send the 4 cameras with predefined viewing positions and directions to the drawing.

        // viewport_space = transform2D::ViewportSpace(0, 0, resolution.x, resolution.y); // default
        viewport_space = transform2D::ViewportSpace(0, 0, resolution.x * 0.75f, resolution.y * 0.5f); // Task 6 -- bottom-left
        DrawObjects(GetSceneCamera(), viewport_space);

        viewport_space = transform2D::ViewportSpace(resolution.x * 0.75f, 0, resolution.x, resolution.y * 0.75f); // Task 6 -- bottom-right
        viewport_space = transform2D::ViewportSpace(resolution.x * 0.75f, resolution.y * 0.75f, resolution.x, resolution.y); // Task 6 -- top-right
        viewport_space = transform2D::ViewportSpace(0, resolution.y * 0.5f, resolution.x * 0.75f, resolution.y); // Task 6 -- top-left


        camera->SetPositionAndRotation(glm::vec3(0, 3, -3), glm::quatLookAt(glm::normalize(glm::vec3(0, -3, 3)), glm::vec3(0, 1, 0)));
        viewport_space = transform2D::ViewportSpace(resolution.x * 0.75f, 0, resolution.x * 0.25f, resolution.y * 0.75f); // Task 6 -- bottom-right
        DrawObjects(camera, viewport_space); // Task 6

        camera->SetPositionAndRotation(glm::vec3(3, 3, 3), glm::quatLookAt(glm::normalize(glm::vec3(-3, -3, -3)), glm::vec3(0, 1, 0)));
        viewport_space = transform2D::ViewportSpace(0, resolution.y * 0.5f, resolution.x * 0.75f, resolution.y * 0.5f); // Task 6 -- top-left
        DrawObjects(camera, viewport_space); // Task 6

        camera->SetPositionAndRotation(glm::vec3(-3, 3, 3), glm::quatLookAt(glm::normalize(glm::vec3(3, -3, -3)), glm::vec3(0, 1, 0)));
        viewport_space = transform2D::ViewportSpace(resolution.x * 0.75f, resolution.y * 0.75f, resolution.x * 0.25f, resolution.y * 0.25f); // Task 6 -- top-right
        DrawObjects(camera, viewport_space); // Task 6
    }
    else
    {
        if (sunCurrX <= sunMinX) {
            sunMoveRight = true;
        }
        if (sunCurrX >= sunMaxX) {
            sunMoveRight = false;
        }
        if (sunMoveRight) {
            sunCurrX += sunSpeed * deltaTimeSeconds;
        }
        else {
            sunCurrX -= sunSpeed * deltaTimeSeconds;
        }
        earthCurrRotation += earthRotationSpeed * deltaTimeSeconds;
        moonCurrRotation += moonRotationSpeed * deltaTimeSeconds;


        glm::ivec2 resolution = window->GetResolution();

        // TODO(student): Set the screen cleaning color. Use the 'color' attribute.
        glClearColor(color.r, color.g, color.b, 1);

        // Clears the color buffer (using the previously set color) and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static auto camera = new gfxc::Camera();

        viewport_space = transform2D::ViewportSpace(0, 0, resolution.x, resolution.y);
        DrawPlanetSystem(GetSceneCamera(), viewport_space);
    }
    
}


void Lab4::FrameEnd()
{

}

void Lab4::DrawObjects(gfxc::Camera *camera, const transform2D::ViewportSpace & viewport_space)
{
    glm::mat4 view = transform3D::View(
        camera->m_transform->GetWorldPosition(),
        camera->m_transform->GetLocalOZVector(),
        camera->m_transform->GetLocalOXVector(),
        camera->m_transform->GetLocalOYVector()
    );

    glm::mat4 projection = transform3D::Perspective(
        glm::radians(60.0f), (float)viewport_space.width / viewport_space.height, 0.1f, 100.0f
    );

    // TODO(student): Enable face culling
    glEnable(GL_CULL_FACE);

    // TODO(student): Set face custom culling. Use the `cullFace` variable.
    glCullFace(cullFace);

    // TODO(student): Set the position and size of the view port based on the
    // information received from the 'viewport_space' parameter.
    glViewport(viewport_space.x, viewport_space.y, viewport_space.width, viewport_space.height);
    

    glm::mat4 model = glm::mat4(1);
    model *= transform3D::Translate(glm::vec3(0, currPos, 0)); // Task 4
    model *= transform3D::Translate(glm::vec3(-1.5f, 0.5f, 0));
    model *= transform3D::RotateOX(glm::radians(45.0f));
    model *= transform3D::RotateOZ(glm::radians(-45.0f));
    model *= transform3D::Scale(glm::vec3(0.75f));
    RenderMesh(meshes["cube"], shaders["VertexColor"], model, view, projection);

    model = glm::mat4(1);
    model *= transform3D::Translate(glm::vec3(0, 0.5f, 0));
    model *= transform3D::Scale(glm::vec3(0.75f));
    model *= transform3D::RotateOY(glm::radians(currRotation)); // Task 4
    RenderMesh(meshes["cube"], shaders["VertexColor"], model, view, projection);

    model = glm::mat4(1);
    model *= transform3D::Translate(glm::vec3(1.5f, 0.5f, 0));
    model *= transform3D::RotateOX(glm::radians(45.0f));
    model *= transform3D::RotateOZ(glm::radians(45.0f));
    model *= transform3D::Scale(glm::vec3(0.5f));
    model *= transform3D::Scale(glm::vec3(currScale)); // Task 4
    RenderMesh(meshes["cube"], shaders["VertexColor"], model, view, projection);

    // Task 5
    model = glm::mat4(1);
    model *= transform3D::Translate(glm::vec3(3.0f, 2.0f, -3.0f));
    model *= transform3D::Translate(glm::vec3(cubePosX, cubePosY, cubePosZ));
    RenderMesh(meshes["cube2"], shaders["VertexColor"], model, view, projection);

    // TODO(student): Disable face culling
    glDisable(GL_CULL_FACE);

    DrawCoordinateSystem(view, projection);
}


// Bonus
void Lab4::DrawPlanetSystem(gfxc::Camera* camera, const transform2D::ViewportSpace& viewport_space)
{
    glm::mat4 view = transform3D::View(
        camera->m_transform->GetWorldPosition(),
        camera->m_transform->GetLocalOZVector(),
        camera->m_transform->GetLocalOXVector(),
        camera->m_transform->GetLocalOYVector()
    );

    glm::mat4 projection = transform3D::Perspective(
        glm::radians(60.0f), (float)viewport_space.width / viewport_space.height, 0.1f, 100.0f
    );

    glEnable(GL_CULL_FACE);
    glCullFace(cullFace);
    glViewport(viewport_space.x, viewport_space.y, viewport_space.width, viewport_space.height);



    // Sun
    glm::mat4 model = glm::mat4(1);
    model *= transform3D::Translate(glm::vec3(sunCurrX, 0, 0));
    RenderMesh(meshes["cube"], shaders["VertexColor"], model, view, projection);


    // Earth
    model = glm::mat4(1);
    model *= transform3D::Translate(glm::vec3(sunCurrX, 0, 0));
    model *= transform3D::RotateOY(RADIANS(earthCurrRotation));
    model *= transform3D::Translate(glm::vec3(sunEarthDistance, 0, 0));
    model *= transform3D::Scale(glm::vec3(0.5f));
    RenderMesh(meshes["cube"], shaders["VertexColor"], model, view, projection);


    // Moon
    model = glm::mat4(1);
    model *= transform3D::Translate(glm::vec3(sunCurrX, 0, 0));
    model *= transform3D::RotateOY(RADIANS(earthCurrRotation));
    model *= transform3D::Translate(glm::vec3(sunEarthDistance, 0, 0));
    model *= transform3D::RotateOY(RADIANS(moonCurrRotation));
    model *= transform3D::Translate(glm::vec3(earthMoonDistance, 0, 0));
    model *= transform3D::Scale(glm::vec3(0.25f));
    RenderMesh(meshes["cube"], shaders["VertexColor"], model, view, projection);


    glDisable(GL_CULL_FACE);
    DrawCoordinateSystem(view, projection);
}



void Lab4::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix,
    const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab4::OnInputUpdate(float deltaTime, int mods)
{
    // TODO(student): Add some key hold events that will let you move
    // a mesh instance on all three axes. You will also need to
    // generalize the position used by `RenderMesh`.
    //
    // Use the 'window->KeyHold()' and 'window->MouseHold()' methods
    // to check if a certain key or mouse button is pressed.
    //
    // Use the 'deltaTime' parameter to make a movement independent
    // of the number of frames rendered per second. More precisely,
    // when we want to move an object at a speed of 5 units per second,
    // we have:
    //     5 units ... 1 second
    //     x units ... deltaTime seconds
    //
    // Use this association to calculate the number of units of
    // movement at the current frame for a given speed per second.
    //
    // Don't change the position of the object when the position
    // of the camera in the scene changes.

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_2)) {
        if (window->KeyHold(GLFW_KEY_A)) {
            cubePosX -= deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_D)) {
            cubePosX += deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_W)) {
            cubePosZ -= deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_S)) {
            cubePosZ += deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_Q)) {
            cubePosY -= deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_E)) {
            cubePosY += deltaTime;
        }
    }

}


void Lab4::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_R) {
        // TODO(student): Change the values of the color components.
        float r, g, b;
        r = rand() % 100 / 100.0f;
        g = rand() % 100 / 100.0f;
        b = rand() % 100 / 100.0f;
        color.r = r;
        color.g = g;
        color.b = b;
    }

    // TODO(student): Switch between GL_FRONT and GL_BACK culling.
    // Save the state in `cullFace` variable and apply it in the
    // `Update()` method, NOT here!
    if (key == GLFW_KEY_F) {
        if (cullFace == GL_BACK) {
            cullFace = GL_FRONT;
        }
        else {
            cullFace = GL_BACK;
        }
    }
}


void Lab4::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab4::OnWindowResize(int width, int height)
{
}
