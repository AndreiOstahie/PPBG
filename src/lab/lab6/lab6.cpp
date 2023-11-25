#include "lab/lab6/lab6.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab6::Lab6()
{
    controlled_light_source_index = 0;

    angle = 0;
}


Lab6::~Lab6()
{
}


void Lab6::Init()
{
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("teapot");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "teapot.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("bunny");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "animals"), "bunny.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "lab6", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "lab6", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        point_light_positions[9] = glm::vec3(0, 1, 1);
        spot_light_positions[9] = glm::vec3(1, 1, 1);

        for (int i = 0; i < 10; i++) {
            spot_light_directions[i] = glm::vec3(0, -1, 0);
            spot_light_angles[i] = glm::radians (10.0f + rand() % 50);

            point_light_colors[i] = glm::vec3(
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f
            );
            spot_light_colors[i] = glm::vec3(
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f
            );
        }
    }
}


void Lab6::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab6::Update(float deltaTimeSeconds)
{
    angle += glm::radians(6.0f) * deltaTimeSeconds;

    for (int i = 0; i < 9; i++) {
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0), angle + i * glm::radians(360.0f) / 9, glm::vec3(0, 1, 0));

        point_light_positions[i] = glm::vec3(glm::mat3(rotation) * glm::vec3(5, 1.5 + sin(Engine::GetElapsedTime() + i/2.0f), 0));
        spot_light_positions[i] = glm::vec3(glm::mat3(rotation) * glm::vec3(3, 1.5 + sin(Engine::GetElapsedTime() + i / 2.0f), 0));
    }

    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes["sphere"], shaders["LabShader"], model, glm::vec3(1, 1, 0));
    }

    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2, 0, 0));
        model = glm::rotate(model, glm::radians(-160.0f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(2.0f));
        RenderSimpleMesh(meshes["teapot"], shaders["LabShader"], model, glm::vec3(1, 0, 1));
    }

    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5, 0, 1.4));
        model = glm::rotate(model, glm::radians(-160.0f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(3.0f));
        RenderSimpleMesh(meshes["teapot"], shaders["LabShader"], model, glm::vec3(1, 1, 0));
    }

    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2, 1.0f, 0));
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(0.05f));
        RenderSimpleMesh(meshes["bunny"], shaders["LabShader"], model, glm::vec3(0, 1, 0));
    }

    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-5, 1.5f, 1));
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(0, 1, 0));
        model = glm::scale(model, glm::vec3(0.1f));
        RenderSimpleMesh(meshes["bunny"], shaders["LabShader"], model, glm::vec3(0, 1, 1));
    }

    // Render ground
    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0, 0.01f, 0));
        model = glm::scale(model, glm::vec3(0.25f));
        RenderSimpleMesh(meshes["plane"], shaders["LabShader"], model, glm::vec3(1));
    }

    // Render the point lights in the scene
    for (int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, point_light_positions[i]);
        model = glm::scale(model, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["Simple"], model);
    }

    // Render the spot lights in the scene
    for (int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, spot_light_positions[i]);
        model = glm::scale(model, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["Simple"], model);
    }
}


void Lab6::FrameEnd()
{
    DrawCoordinateSystem();
}

void Lab6::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & model, const glm::vec3 &object_color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Send shader uniforms for light & material properties

    // TODO(student): Send the information about the light sources
    // (positions of the point light sources, colors of the point light
    // sources, positions of the spot light sources, colors of the spot light
    // sources, directions of the spot light sources and angles for the
    // spot light sources) in attributes of uniform type. Use the attributes
    // defined in "lab6.h". Send 10 entities of each.
    int location = 0;
    location = glGetUniformLocation(shader->program, "point_light_positions");
    glUniform3fv(location, 10, glm::value_ptr(point_light_positions[0]));
    location = glGetUniformLocation(shader->program, "spot_light_positions");
    glUniform3fv(location, 10, glm::value_ptr(spot_light_positions[0]));
    location = glGetUniformLocation(shader->program, "spot_light_directions");
    glUniform3fv(location, 10, glm::value_ptr(spot_light_directions[0]));

    location = glGetUniformLocation(shader->program, "point_light_colors");
    glUniform3fv(location, 10, glm::value_ptr(point_light_colors[0]));
    location = glGetUniformLocation(shader->program, "spot_light_colors");
    glUniform3fv(location, 10, glm::value_ptr(spot_light_colors[0]));
    location = glGetUniformLocation(shader->program, "spot_light_angles");
    glUniform1fv(location, 10, spot_light_angles);


    glm::vec3 eye_position = GetSceneCamera()->m_transform->GetWorldPosition();

    // TODO(student): Set eye position (camera position) uniform
    int eye_position_location = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(eye_position_location, 1, glm::value_ptr(eye_position));


    glm::vec3 material_ka = object_color;
    glm::vec3 material_kd = object_color;
    glm::vec3 material_ks = object_color;
    int material_shininess = 30;

    // TODO(student): Set material property uniforms (shininess, ka, kd, ks)
    location = glGetUniformLocation(shader->program, "material_ka");
    glUniform3fv(location, 1, glm::value_ptr(material_ka));
    location = glGetUniformLocation(shader->program, "material_kd");
    glUniform3fv(location, 1, glm::value_ptr(material_kd));
    location = glGetUniformLocation(shader->program, "material_ks");
    glUniform3fv(location, 1, glm::value_ptr(material_ks));
    location = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(location, material_shininess);


    // Send the model matrix uniform
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(model));

    // Send the view matrix unfirom
    glm::mat4 view = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));

    // Send the projection matrix uniform
    glm::mat4 projection = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab6::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        const float speed = 2;

        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

        glm::vec3* light_position = nullptr;

        if (controlled_light_source_index == 0) {
            light_position = &point_light_positions[9];
        }

        if (controlled_light_source_index == 1) {
            light_position = &spot_light_positions[9];
        }

        // Control light position using on W, A, S, D, E, Q
        if (window->KeyHold(GLFW_KEY_W)) (*light_position) -= forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_A)) (*light_position) -= right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_S)) (*light_position) += forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_D)) (*light_position) += right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_E)) (*light_position) += up * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_Q)) (*light_position) -= up * deltaTime * speed;
    }

    {
        glm::vec3 &light_direction = spot_light_directions[9];
        float &angle = spot_light_angles[9];
        // TODO(student): Change the lighting direction and angle of the spot
        // light source from the keyboard. From the keys, implement the possibility
        // of rotating the lighting direction relative to the OX and OZ axes, in both
        // directions and the possibility of increasing and decreasing the angle.

        if (window->KeyHold(GLFW_KEY_UP)) {
            glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(50 * deltaTime), glm::vec3(1, 0, 0));
            light_direction = glm::mat3(rotation) * light_direction;
        }
        if (window->KeyHold(GLFW_KEY_DOWN)) {
            glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(-50 * deltaTime), glm::vec3(1, 0, 0));
            light_direction = glm::mat3(rotation) * light_direction;
        }
        if (window->KeyHold(GLFW_KEY_LEFT)) {
            glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(-50 * deltaTime), glm::vec3(0, 0, 1));
            light_direction = glm::mat3(rotation) * light_direction;
        }
        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(50 * deltaTime), glm::vec3(0, 0, 1));
            light_direction = glm::mat3(rotation) * light_direction;
        }

        if (window->KeyHold(GLFW_KEY_Z)) {
            angle -= deltaTime;
        }
        if (window->KeyHold(GLFW_KEY_X)) {
            angle += deltaTime;
        }
    }
}


void Lab6::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        controlled_light_source_index = (controlled_light_source_index + 1) % 2;
    }
}


void Lab6::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab6::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab6::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab6::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab6::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab6::OnWindowResize(int width, int height)
{
}
