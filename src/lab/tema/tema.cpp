#include "lab/tema/tema.h"
#include "lab/tema/enemy.h"

#include <vector>
#include <string>
#include <iostream>

#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/random.hpp>

#include "lab/lab4/transform3D.h"

using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema::Tema()
{
    controlled_light_source_index = 0;

    angle = 0;
}


Tema::~Tema()
{
}


void Tema::Init()
{
    // Create a ground mesh using custom data
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, 0, -1), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(-1, 0, 1), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(1, 0, 1), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(1, 0, -1), glm::vec3(1, 0, 0))   
        };

        vector<unsigned int> indices
        {
            0, 1, 2,    // indices for first triangle
            2, 0, 3
        };

        CreateMesh("ground", vertices, indices);
    }

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

        CreateMesh("cube", vertices, indices);
    }

    // Load meshes
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
    }
    

    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader* shader = new Shader("SimpleShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "SimpleShader.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders", "SimpleShader.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Light & material properties
    {
        point_light_positions[9] = glm::vec3(0, 1, 1);
        for (int i = 0; i < 10; i++) {
            point_light_colors[i] = glm::vec3(
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f,
                rand() % 256 / 255.0f
            );
        }
    }


    // Variables init
    duoModeEnabled = false;

    lightpost_color = glm::vec3(1, 0, 0);
    ground_color = glm::vec3(0, 1, 0);
    player_color = glm::vec3(0, 0, 1);

    playerRotY = 0.0f;
    playerPos = glm::vec3(0, 0, 0);
    playerModelMat = glm::mat4(0);
    playerSpeed = 15.0f;
    limbRotation1 = 0.0f;
    limbRotation2 = 0.0f;
    limbMaxRotation = 45.0f;
    limbMinRotation = -45.0f;
    limbDirection1 = 0;
    limbDirection2 = 1;
    limbRotationSpeed = 200.0f;
    isMoving = false;
    cameraOffset = glm::vec3(0, 10, 10);
    curveFactor = 0.02f;

    timeSinceLastRotation = 0.0f;
    timeBetweenRotations = 1.0f;
    spot_rotation_speed = 0.5f;
    spot_rotation_max_cap = 180.0f;

    groundScale = 50.0f;
    lightposts_per_row = 5;
    map_border = 5.0f;
    glm::vec3 minBounds(-groundScale, 0.0f, -groundScale);
    glm::vec3 maxBounds(groundScale, 5.0f, groundScale);

    attackRadius = 5.0f;

    enemyCount = 50;
    enemies = (Enemy *) malloc(enemyCount * sizeof(Enemy));

    secondCamera = new gfxc::Camera();

    // Second player init
    second_player_color = glm::vec3(1, 1, 0);
    second_playerRotY = 0.0f;
    second_playerPos = glm::vec3(0, 0, 0);
    second_playerModelMat = glm::mat4(0);
    second_limbRotation1 = 0.0f;
    second_limbRotation2 = 0.0f;
    second_limbDirection1 = 0;
    second_limbDirection2 = 1;
    second_isMoving = false;



    // Create enemies
    for (int i = 0; i < enemyCount; i++)
    {
        float randomX = minBounds.x + static_cast<float>(rand()) / RAND_MAX * (maxBounds.x - minBounds.x);
        float randomZ = minBounds.z + static_cast<float>(rand()) / RAND_MAX * (maxBounds.z - minBounds.z);

        float r, g, b;
        r = rand() % 100 / 100.0f;
        g = rand() % 100 / 100.0f;
        b = rand() % 100 / 100.0f;

        enemies[i] = Enemy(glm::vec3(randomX, 0, randomZ), glm::vec3(r, g, b));
    }

    // Determine light posts spawn positions
    lightpost_distance = ((groundScale - map_border) - (-groundScale + map_border)) / (lightposts_per_row - 1);
    lightpost_positions = (glm::vec3 *)malloc(lightposts_per_row * lightposts_per_row * sizeof(glm::vec3));
    int index = 0;
    for (int i = 0; i < lightposts_per_row; i++)
    {
        float startPosX = -groundScale + map_border;
        float startPosY = -groundScale + map_border;
        for (int j = 0; j < lightposts_per_row; j++)
        {
            lightpost_positions[index++] = glm::vec3(startPosX + j * lightpost_distance, 0, startPosY + i * lightpost_distance);
        }
    }

    // Set up spotlights for lightposts
    spot_count = lightposts_per_row * lightposts_per_row * 2 + 2;

    spot_positions = (glm::vec3 *) malloc(spot_count * sizeof(glm::vec3));
    spot_directions = (glm::vec3 *) malloc(spot_count * sizeof(glm::vec3));
    spot_colors = (glm::vec3 *) malloc(spot_count * sizeof(glm::vec3));
    spot_angles = (float *) malloc(spot_count * sizeof(glm::vec3));
    spot_rotations = (glm::vec2 *) malloc(spot_count * sizeof(glm::vec2));

    int spot_index = 0;
    for (int i = 0; i < lightposts_per_row * lightposts_per_row; i++)
    {
        spot_positions[spot_index] = lightpost_positions[i] + glm::vec3(-0.55f, 4.75f, -1.0f);
        spot_directions[spot_index] = glm::vec3(0, -1, 0);
        spot_angles[spot_index] = 45.0f;
        spot_colors[spot_index] = glm::vec3(1, 1, 1);
        spot_index++;

        spot_positions[spot_index] = lightpost_positions[i] + glm::vec3(0.55f, 4.75f, 1.0f);
        spot_directions[spot_index] = glm::vec3(0, -1, 0);
        spot_angles[spot_index] = 45.0f;
        spot_colors[spot_index] = glm::vec3(1, 1, 1);
        spot_index++;
    }
    spot_positions[spot_index] = playerPos + glm::vec3(0, 3.0f, 0);
    spot_directions[spot_index] = glm::vec3(0, 0, -1);
    spot_angles[spot_index] = 60.0f;
    spot_colors[spot_index] = glm::vec3(1, 1, 1);

    spot_positions[spot_index + 1] = second_playerPos + glm::vec3(0, 3.0f, 0);
    spot_directions[spot_index + 1] = glm::vec3(0, 0, -1);
    spot_angles[spot_index + 1] = 60.0f;
    spot_colors[spot_index + 1] = glm::vec3(1, 1, 1);

    // Generate random rotations for each spotlight
    for (int i = 0; i < spot_count - 2; i++)
    {
        float angle_x = glm::linearRand(0.0f, spot_rotation_max_cap); // Random rotation around X axis
        float angle_z = glm::linearRand(0.0f, spot_rotation_max_cap); // Random rotation around Z axis

        spot_rotations[i] = glm::vec2(angle_x, angle_z);
    }

}


void Tema::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema::Update(float deltaTimeSeconds)
{
    //GetSceneCamera()->SetPosition(playerPos + cameraOffset);
    //GetSceneCamera()->SetRotation(glm::angleAxis(RADIANS(-30.0f), glm::vec3(1, 0, 0)));
    
    //std::cout << "Position: " << glm::to_string(spot_positions[spot_count - 1]) << std::endl;
    //std::cout << "Direction: " << glm::to_string(spot_directions[spot_count - 1]) << std::endl;


    // Spotlights rotation

    // Update the rotation timer
    timeSinceLastRotation += deltaTimeSeconds;

    if (timeSinceLastRotation >= timeBetweenRotations)
    {
        // Reset timer
        timeSinceLastRotation = 0.0f;

        // Generate random rotations for each spotlight
        for (int i = 0; i < spot_count - 2; i++)
        {
            float angle_x = glm::linearRand(0.0f, spot_rotation_max_cap); // Random rotation around X axis
            float angle_z = glm::linearRand(0.0f, spot_rotation_max_cap); // Random rotation around Z axis

            spot_rotations[i] = glm::vec2(angle_x, angle_z);
        }
    }
    else
    {
        for (int i = 0; i < spot_count - 2; i++)
        {
            // Apply rotations
            glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(spot_rotations[i].x * deltaTimeSeconds * spot_rotation_speed), glm::vec3(1, 0, 0))
                                * glm::rotate(glm::mat4(1), glm::radians(spot_rotations[i].y * deltaTimeSeconds * spot_rotation_speed), glm::vec3(0, 0, 1));

            // Update spot light direction
            spot_directions[i] = glm::mat3(rotation) * spot_directions[i];
        }
    }


    // Enemy movement
    {
        for (int i = 0; i < enemyCount; i++)
        {
            float distanceToMainPlayer = glm::length(enemies[i].GetPosition() - playerPos);
            float distanceToSecondPlayer = glm::length(enemies[i].GetPosition() - second_playerPos);

            // Enemies follow player
            if (distanceToSecondPlayer <= distanceToMainPlayer && duoModeEnabled)
            {
                enemies[i].Follow(second_playerPos, deltaTimeSeconds);
            }
            else
            {
                enemies[i].Follow(playerPos, deltaTimeSeconds);
            }

            enemies[i].Animate(deltaTimeSeconds);
        }
    }

    if (duoModeEnabled)
    {
        // Set camera position to follow main player
        auto camera = GetSceneCamera();
        camera->SetPositionAndRotation(
            playerPos + cameraOffset,
            glm::quatLookAt(-glm::normalize(cameraOffset), glm::vec3(0, 1, 0))
        );

        glm::ivec2 resolution = window->GetResolution();
        viewport_space = transform2D::ViewportSpace(0, 0, resolution.x * 0.5f, resolution.y);
        DrawScene(camera, viewport_space, deltaTimeSeconds);


        // Set camera position to follow second player
        secondCamera->SetPositionAndRotation(
            second_playerPos + cameraOffset,
            glm::quatLookAt(-glm::normalize(cameraOffset), glm::vec3(0, 1, 0))
        );

        viewport_space = transform2D::ViewportSpace(resolution.x * 0.5f, 0, resolution.x * 0.5f, resolution.y);
        DrawScene(secondCamera, viewport_space, deltaTimeSeconds);
    }
    else
    {
        // Set camera position to follow main player
        auto camera = GetSceneCamera();
        camera->SetPositionAndRotation(
            playerPos + cameraOffset,
            glm::quatLookAt(-glm::normalize(cameraOffset), glm::vec3(0, 1, 0))
        );
        glm::ivec2 resolution = window->GetResolution();
        viewport_space = transform2D::ViewportSpace(0, 0, resolution.x, resolution.y);
        DrawScene(GetSceneCamera(), viewport_space, deltaTimeSeconds);
    }
    

    
}


void Tema::Attack(bool mainPlayer)
{
    if (mainPlayer)
    {
        // Get player forward vector based on Y rotation
        glm::vec3 forwardVector = glm::vec3(glm::sin(glm::radians(playerRotY)), 0, glm::cos(glm::radians(playerRotY)));

        glm::vec3 circleCenter = playerPos + attackRadius * glm::normalize(forwardVector);

        glm::vec3 minBounds(-groundScale, 0.0f, -groundScale);
        glm::vec3 maxBounds(groundScale, 5.0f, groundScale);

        for (int i = 0; i < enemyCount; i++)
        {
            float distance = glm::length(enemies[i].GetPosition() - circleCenter);

            if (distance <= attackRadius)
            {
                printf("Enemy detected\n");
                float randomX = minBounds.x + static_cast<float>(rand()) / RAND_MAX * (maxBounds.x - minBounds.x);
                float randomZ = minBounds.z + static_cast<float>(rand()) / RAND_MAX * (maxBounds.z - minBounds.z);
                enemies[i].SetPosition(glm::vec3(randomX, 0, randomZ));
            }
        }
    }
    else
    {
        // Get player forward vector based on Y rotation
        glm::vec3 forwardVector = glm::vec3(glm::sin(glm::radians(second_playerRotY)), 0, glm::cos(glm::radians(second_playerRotY)));

        glm::vec3 circleCenter = second_playerPos + attackRadius * glm::normalize(forwardVector);

        glm::vec3 minBounds(-groundScale, 0.0f, -groundScale);
        glm::vec3 maxBounds(groundScale, 5.0f, groundScale);

        for (int i = 0; i < enemyCount; i++)
        {
            float distance = glm::length(enemies[i].GetPosition() - circleCenter);

            if (distance <= attackRadius)
            {
                printf("Enemy detected\n");
                float randomX = minBounds.x + static_cast<float>(rand()) / RAND_MAX * (maxBounds.x - minBounds.x);
                float randomZ = minBounds.z + static_cast<float>(rand()) / RAND_MAX * (maxBounds.z - minBounds.z);
                enemies[i].SetPosition(glm::vec3(randomX, 0, randomZ));
            }
        }
    }
    
}



void Tema::DrawScene(gfxc::Camera* camera, const transform2D::ViewportSpace& viewport_space, float deltaTimeSeconds)
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


    // TODO(student): Set the position and size of the view port based on the
    // information received from the 'viewport_space' parameter.
    glViewport(viewport_space.x, viewport_space.y, viewport_space.width, viewport_space.height);


    bool useMainCamera = true;
    if (camera == secondCamera)
    {
        useMainCamera = false;
    }
    
    
    // Render ground
    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0, 0.1f, 0));
        model = glm::scale(model, glm::vec3(groundScale));
        RenderSimpleMesh(meshes["ground"], shaders["SimpleShader"], model, view, projection, ground_color, useMainCamera);
    }

    // Render Player Model
    {
        // Body
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(0, 1.55f, 0)); // translate limb relative to player's body
            model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.25f));
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, player_color, useMainCamera);
        }

        // Head
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(0, 2.2f, 0)); // translate limb relative to player's body
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, player_color, useMainCamera);
        }

        // Right Arm
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(0.4f, 2.05f, 0)); // translate limb relative to player's body
            model = glm::rotate(model, RADIANS(limbRotation1), glm::vec3(1, 0, 0)); // rotate limb to desired angle
            model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
            model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, player_color, useMainCamera);
        }
        // Left Arm
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(-0.4f, 2.05f, 0)); // translate limb relative to player's body
            model = glm::rotate(model, RADIANS(limbRotation2), glm::vec3(1, 0, 0)); // rotate limb to desired angle
            model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
            model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, player_color, useMainCamera);
        }

        // Right Leg
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(0.15f, 1.0f, 0)); // translate limb relative to player's body
            model = glm::rotate(model, RADIANS(limbRotation2), glm::vec3(1, 0, 0)); // rotate limb to desired angle
            model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
            model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, player_color, useMainCamera);
        }
        // Left Leg
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(-0.15f, 1.0f, 0)); // translate limb relative to player's body
            model = glm::rotate(model, RADIANS(limbRotation1), glm::vec3(1, 0, 0)); // rotate limb to desired angle
            model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
            model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, player_color, useMainCamera);
        }

        // Spot Light
        {
            spot_positions[spot_count - 2] = playerPos + glm::vec3(0, 3.0f, 0);
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, spot_positions[spot_count - 2]);
            model = glm::scale(model, glm::vec3(0.1f));
            // RenderMesh(meshes["sphere"], shaders["Simple"], model);
        }

        // Point Light
        {
            point_light_positions[8] = playerPos + glm::vec3(0, 2.5f, 1);
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, point_light_positions[8]);
            model = glm::scale(model, glm::vec3(5.1f));
            // RenderMesh(meshes["sphere"], shaders["Simple"], model);
        }
    }

    // Render Second Player Model
    if (duoModeEnabled)
    {
        // Body
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, second_playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(second_playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(0, 1.55f, 0)); // translate limb relative to player's body
            model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.25f));
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, second_player_color, useMainCamera);
        }

        // Head
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, second_playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(second_playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(0, 2.2f, 0)); // translate limb relative to player's body
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, second_player_color, useMainCamera);
        }

        // Right Arm
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, second_playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(second_playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(0.4f, 2.05f, 0)); // translate limb relative to player's body
            model = glm::rotate(model, RADIANS(second_limbRotation1), glm::vec3(1, 0, 0)); // rotate limb to desired angle
            model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
            model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, second_player_color, useMainCamera);
        }
        // Left Arm
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, second_playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(second_playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(-0.4f, 2.05f, 0)); // translate limb relative to player's body
            model = glm::rotate(model, RADIANS(second_limbRotation2), glm::vec3(1, 0, 0)); // rotate limb to desired angle
            model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
            model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, second_player_color, useMainCamera);
        }

        // Right Leg
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, second_playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(second_playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(0.15f, 1.0f, 0)); // translate limb relative to player's body
            model = glm::rotate(model, RADIANS(second_limbRotation2), glm::vec3(1, 0, 0)); // rotate limb to desired angle
            model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
            model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, second_player_color, useMainCamera);
        }
        // Left Leg
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, second_playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(second_playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(-0.15f, 1.0f, 0)); // translate limb relative to player's body
            model = glm::rotate(model, RADIANS(second_limbRotation1), glm::vec3(1, 0, 0)); // rotate limb to desired angle
            model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
            model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, second_player_color, useMainCamera);
        }
        // Spot Light
        {
            spot_positions[spot_count - 1] = second_playerPos + glm::vec3(0, 3.0f, 0);
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, spot_positions[spot_count - 1]);
            model = glm::scale(model, glm::vec3(0.1f));
            // RenderMesh(meshes["sphere"], shaders["Simple"], model);
        }

        // Point Light
        {
            point_light_positions[9] = second_playerPos + glm::vec3(0, 2.5f, 1);
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, point_light_positions[9]);
            model = glm::scale(model, glm::vec3(5.1f));
            // RenderMesh(meshes["sphere"], shaders["Simple"], model);
        }

        
    }

    // Render light posts and spot lights
    for (int i = 0; i < lightposts_per_row * lightposts_per_row; i++)
    {
        // Light posts
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, lightpost_positions[i]);
        model = glm::rotate(model, RADIANS(30), glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(0, 5.0f / 2, 0));
        model = glm::scale(model, glm::vec3(0.35f, 5.0f, 0.35f));
        RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, lightpost_color, useMainCamera);

        model = glm::mat4(1);
        model = glm::translate(model, lightpost_positions[i]);
        model = glm::rotate(model, RADIANS(30), glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(0, 5.0f, 0));
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 3.0f));
        RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, lightpost_color, useMainCamera);


        // Spot lights
        model = glm::mat4(1);
        model = glm::translate(model, spot_positions[i * 2]);
        model = glm::scale(model, glm::vec3(0.1f));
        // RenderMesh(meshes["sphere"], shaders["Simple"], model);
        RenderSimpleMesh(meshes["sphere"], shaders["SimpleShader"], model, view, projection, glm::vec3(1, 1, 1), useMainCamera);

        model = glm::mat4(1);
        model = glm::translate(model, spot_positions[i * 2 + 1]);
        model = glm::scale(model, glm::vec3(0.1f));
        // RenderMesh(meshes["sphere"], shaders["Simple"], model);
        RenderSimpleMesh(meshes["sphere"], shaders["SimpleShader"], model, view, projection, glm::vec3(1, 1, 1), useMainCamera);
    }

    // Enemies
    {
        for (int i = 0; i < enemyCount; i++)
        {
            // Render enemies
            {
                // Body
                {
                    glm::mat4 model = glm::mat4(1);
                    model = glm::translate(model, enemies[i].GetPosition()); // translate limb to enemy's current pos
                    model = glm::rotate(model, RADIANS(enemies[i].GetRotation().y), glm::vec3(0, 1, 0)); // enemy rotation
                    model = glm::translate(model, glm::vec3(0, 1.55f, 0)); // translate limb relative to enemy's body
                    model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.25f));
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, enemies[i].GetColor(), useMainCamera);
                }

                // Head
                {
                    glm::mat4 model = glm::mat4(1);
                    model = glm::translate(model, enemies[i].GetPosition()); // translate limb to enemy's current pos
                    model = glm::rotate(model, RADIANS(enemies[i].GetRotation().y), glm::vec3(0, 1, 0)); // enemy rotation
                    model = glm::translate(model, glm::vec3(0, 2.2f, 0)); // translate limb relative to enemy's body
                    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, enemies[i].GetColor(), useMainCamera);
                }

                // Right Arm
                {
                    glm::mat4 model = glm::mat4(1);
                    model = glm::translate(model, enemies[i].GetPosition()); // translate limb to enemy's current pos
                    model = glm::rotate(model, RADIANS(enemies[i].GetRotation().y), glm::vec3(0, 1, 0)); // enemy rotation
                    model = glm::translate(model, glm::vec3(0.4f, 2.05f, 0)); // translate limb relative to enemy's body
                    model = glm::rotate(model, RADIANS(enemies[i].GetLimbRotation1()), glm::vec3(1, 0, 0)); // rotate limb to desired angle
                    model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
                    model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, enemies[i].GetColor(), useMainCamera);
                }
                // Left Arm
                {
                    glm::mat4 model = glm::mat4(1);
                    model = glm::translate(model, enemies[i].GetPosition()); // translate limb to enemy's current pos
                    model = glm::rotate(model, RADIANS(enemies[i].GetRotation().y), glm::vec3(0, 1, 0)); // enemy rotation
                    model = glm::translate(model, glm::vec3(-0.4f, 2.05f, 0)); // translate limb relative to enemy's body
                    model = glm::rotate(model, RADIANS(enemies[i].GetLimbRotation2()), glm::vec3(1, 0, 0)); // rotate limb to desired angle
                    model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
                    model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, enemies[i].GetColor(), useMainCamera);
                }

                // Right Leg
                {
                    glm::mat4 model = glm::mat4(1);
                    model = glm::translate(model, enemies[i].GetPosition()); // translate limb to enemy's current pos
                    model = glm::rotate(model, RADIANS(enemies[i].GetRotation().y), glm::vec3(0, 1, 0)); // enemy rotation
                    model = glm::translate(model, glm::vec3(0.15f, 1.0f, 0)); // translate limb relative to enemy's body
                    model = glm::rotate(model, RADIANS(enemies[i].GetLimbRotation2()), glm::vec3(1, 0, 0)); // rotate limb to desired angle
                    model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
                    model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, enemies[i].GetColor(), useMainCamera);
                }
                // Left Leg
                {
                    glm::mat4 model = glm::mat4(1);
                    model = glm::translate(model, enemies[i].GetPosition()); // translate limb to enemy's current pos
                    model = glm::rotate(model, RADIANS(enemies[i].GetRotation().y), glm::vec3(0, 1, 0)); // enemy rotation
                    model = glm::translate(model, glm::vec3(-0.15f, 1.0f, 0)); // translate limb relative to enemy's body
                    model = glm::rotate(model, RADIANS(enemies[i].GetLimbRotation1()), glm::vec3(1, 0, 0)); // rotate limb to desired angle
                    model = glm::scale(model, glm::vec3(0.2f, 1.0f, 0.2f)); // scale limb to desired ratio
                    model = glm::translate(model, glm::vec3(0, -0.5f, 0)); // translate limb so rotation pivot is correct
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, view, projection, enemies[i].GetColor(), useMainCamera);
                }
            }
        }
    }
}



void Tema::FrameEnd()
{
    // DrawCoordinateSystem();
}

void Tema::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& object_color, bool useMainCamera)
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


    // Send light posts spotlight data
    location = glGetUniformLocation(shader->program, "spot_count");
    glUniform1i(location, spot_count);
    location = glGetUniformLocation(shader->program, "spot_positions");
    glUniform3fv(location, spot_count, glm::value_ptr(spot_positions[0]));
    location = glGetUniformLocation(shader->program, "spot_directions");
    glUniform3fv(location, spot_count, glm::value_ptr(spot_directions[0]));
    location = glGetUniformLocation(shader->program, "spot_colors");
    glUniform3fv(location, spot_count, glm::value_ptr(spot_colors[0]));
    location = glGetUniformLocation(shader->program, "spot_angles");
    glUniform1fv(location, spot_count, spot_angles);

    glm::vec3 eye_position;

    if (useMainCamera)
    {
        eye_position = GetSceneCamera()->m_transform->GetWorldPosition();
    }
    else
    {
        eye_position = secondCamera->m_transform->GetWorldPosition();
    }

    

    // TODO(student): Set eye position (camera position) uniform
    int eye_position_location = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(eye_position_location, 1, glm::value_ptr(eye_position));

    int playerPos_location = glGetUniformLocation(shader->program, "playerPos");
    glUniform3fv(playerPos_location, 1, glm::value_ptr(playerPos));

    playerModelMat = glm::mat4(1);
    playerModelMat = glm::translate(playerModelMat, playerPos); // Player model matrix for shader when creating curvature
    // playerModelMat = glm::rotate(playerModelMat, RADIANS(playerRotY), glm::vec3(0, 1, 0)); // player rotation
    GLint loc_playerModelMat = glGetUniformLocation(shader->program, "playerModelMat");
    glUniformMatrix4fv(loc_playerModelMat, 1, GL_FALSE, glm::value_ptr(playerModelMat));

    int curveFactor_location = glGetUniformLocation(shader->program, "curveFactor");
    glUniform1f(curveFactor_location, curveFactor);



    // std::cout << glm::to_string(playerModelMat) << std::endl;




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
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(view));

    // Send the projection matrix uniform
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


Mesh* Tema::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data.

    // To get an idea about how they're different from one another, do the
    // following experiments. What happens if you switch the color pipe and
    // normal pipe in this function (but not in the shader)? Now, what happens
    // if you do the same thing in the shader (but not in this function)?
    // Finally, what happens if you do the same thing in both places? Why?

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

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema::OnInputUpdate(float deltaTime, int mods)
{
    // Player Movement Input
    {
        glm::vec3 movement(0.0f, 0.0f, 0.0f);

        if (window->KeyHold(GLFW_KEY_A))
        {
            movement.x -= 1.0f;
        }
        if (window->KeyHold(GLFW_KEY_D))
        {
            movement.x += 1.0f;
        }
        if (window->KeyHold(GLFW_KEY_S))
        {
            movement.z += 1.0f;
        }
        if (window->KeyHold(GLFW_KEY_W))
        {
            movement.z -= 1.0f;
        }

        // Determine player rotation based on movement
        // Forward
        if (movement.x == 0 && movement.z < 0)
        {
            playerRotY = 180.0f;
            spot_directions[spot_count - 2] = glm::vec3(0, 0, -1);
        }
        // Forward-left
        if (movement.x < 0 && movement.z < 0)
        {
            playerRotY = 180.0f + 45.0f;
            spot_directions[spot_count - 2] = glm::vec3(-1, 0, -1);
        }
        // Forward-right
        if (movement.x > 0 && movement.z < 0)
        {
            playerRotY = 180.0f - 45.0f;
            spot_directions[spot_count - 2] = glm::vec3(1, 0, -1);
        }
        // Left
        if (movement.x < 0 && movement.z == 0)
        {
            playerRotY = -90.0f;
            spot_directions[spot_count - 2] = glm::vec3(-1, 0, 0);
        }
        // Right
        if (movement.x > 0 && movement.z == 0)
        {
            playerRotY = 90.0f;
            spot_directions[spot_count - 2] = glm::vec3(1, 0, 0);
        }
        // Back
        if (movement.x == 0 && movement.z > 0)
        {
            playerRotY = 0.0f;
            spot_directions[spot_count - 2] = glm::vec3(0, 0, 1);
        }
        // Back-left
        if (movement.x < 0 && movement.z > 0)
        {
            playerRotY = -45.0f;
            spot_directions[spot_count - 2] = glm::vec3(-1, 0, 1);
        }
        // Back-right
        if (movement.x > 0 && movement.z > 0)
        {
            playerRotY = 45.0f;
            spot_directions[spot_count - 2] = glm::vec3(1, 0, 1);
        }

        // Normalize the movement vector
        if (glm::length(movement) > 0.0f)
        {
            movement = glm::normalize(movement);
        }

        if (movement != glm::vec3(0))
        {
            isMoving = true;

            // Update player position
            playerPos += movement * deltaTime * playerSpeed;

            // Calculate limb rotation for walking animation
            if (limbDirection1 == 0)
            {
                limbRotation1 += deltaTime * limbRotationSpeed;
            }
            else
            {
                limbRotation1 -= deltaTime * limbRotationSpeed;
            }
            if (limbDirection2 == 0)
            {
                limbRotation2 += deltaTime * limbRotationSpeed;
            }
            else
            {
                limbRotation2 -= deltaTime * limbRotationSpeed;
            }

            if (limbRotation1 >= limbMaxRotation)
            {
                limbDirection1 = 1;
            }
            if (limbRotation1 <= limbMinRotation)
            {
                limbDirection1 = 0;
            }
            if (limbRotation2 >= limbMaxRotation)
            {
                limbDirection2 = 1;
            }
            if (limbRotation2 <= limbMinRotation)
            {
                limbDirection2 = 0;
            }
        }
        else
        {
            isMoving = false;

            // Calculate limb rotation for idle player
            const float resetSpeed = 5.0f;
            if (limbRotation1 != 0 && limbRotation2 != 0)
            {
                limbRotation1 = glm::mix(limbRotation1, 0.0f, deltaTime * resetSpeed);
                limbRotation2 = glm::mix(limbRotation2, 0.0f, deltaTime * resetSpeed);
            }
        }
        
    }

    // Second Player Movement Input
    if(duoModeEnabled)
    {
        glm::vec3 movement(0.0f, 0.0f, 0.0f);

        if (window->KeyHold(GLFW_KEY_LEFT))
        {
            movement.x -= 1.0f;
        }
        if (window->KeyHold(GLFW_KEY_RIGHT))
        {
            movement.x += 1.0f;
        }
        if (window->KeyHold(GLFW_KEY_DOWN))
        {
            movement.z += 1.0f;
        }
        if (window->KeyHold(GLFW_KEY_UP))
        {
            movement.z -= 1.0f;
        }

        // Determine player rotation based on movement
        // Forward
        if (movement.x == 0 && movement.z < 0)
        {
            second_playerRotY = 180.0f;
            spot_directions[spot_count - 1] = glm::vec3(0, 0, -1);
        }
        // Forward-left
        if (movement.x < 0 && movement.z < 0)
        {
            second_playerRotY = 180.0f + 45.0f;
            spot_directions[spot_count - 1] = glm::vec3(-1, 0, -1);
        }
        // Forward-right
        if (movement.x > 0 && movement.z < 0)
        {
            second_playerRotY = 180.0f - 45.0f;
            spot_directions[spot_count - 1] = glm::vec3(1, 0, -1);
        }
        // Left
        if (movement.x < 0 && movement.z == 0)
        {
            second_playerRotY = -90.0f;
            spot_directions[spot_count - 1] = glm::vec3(-1, 0, 0);
        }
        // Right
        if (movement.x > 0 && movement.z == 0)
        {
            second_playerRotY = 90.0f;
            spot_directions[spot_count - 1] = glm::vec3(1, 0, 0);
        }
        // Back
        if (movement.x == 0 && movement.z > 0)
        {
            second_playerRotY = 0.0f;
            spot_directions[spot_count - 1] = glm::vec3(0, 0, 1);
        }
        // Back-left
        if (movement.x < 0 && movement.z > 0)
        {
            second_playerRotY = -45.0f;
            spot_directions[spot_count - 1] = glm::vec3(-1, 0, 1);
        }
        // Back-right
        if (movement.x > 0 && movement.z > 0)
        {
            second_playerRotY = 45.0f;
            spot_directions[spot_count - 1] = glm::vec3(1, 0, 1);
        }

        // Normalize the movement vector
        if (glm::length(movement) > 0.0f)
        {
            movement = glm::normalize(movement);
        }

        if (movement != glm::vec3(0))
        {
            second_isMoving = true;

            // Update player position
            second_playerPos += movement * deltaTime * playerSpeed;

            // Calculate limb rotation for walking animation
            if (second_limbDirection1 == 0)
            {
                second_limbRotation1 += deltaTime * limbRotationSpeed;
            }
            else
            {
                second_limbRotation1 -= deltaTime * limbRotationSpeed;
            }
            if (second_limbDirection2 == 0)
            {
                second_limbRotation2 += deltaTime * limbRotationSpeed;
            }
            else
            {
                second_limbRotation2 -= deltaTime * limbRotationSpeed;
            }

            if (second_limbRotation1 >= limbMaxRotation)
            {
                second_limbDirection1 = 1;
            }
            if (second_limbRotation1 <= limbMinRotation)
            {
                second_limbDirection1 = 0;
            }
            if (second_limbRotation2 >= limbMaxRotation)
            {
                second_limbDirection2 = 1;
            }
            if (second_limbRotation2 <= limbMinRotation)
            {
                second_limbDirection2 = 0;
            }
        }
        else
        {
            second_isMoving = false;

            // Calculate limb rotation for idle player
            const float resetSpeed = 5.0f;
            if (second_limbRotation1 != 0 && second_limbRotation2 != 0)
            {
                second_limbRotation1 = glm::mix(second_limbRotation1, 0.0f, deltaTime * resetSpeed);
                second_limbRotation2 = glm::mix(second_limbRotation2, 0.0f, deltaTime * resetSpeed);
            }
        }

    }
}


void Tema::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        controlled_light_source_index = (controlled_light_source_index + 1) % 2;
    }

    if (key == GLFW_KEY_SPACE)
    {
        // Main player attack
        Attack(true);
    }
    if (key == GLFW_KEY_ENTER)
    {
        // Main player attack
        Attack(false);
    }

    if (key == GLFW_KEY_P)
    {
        if (!duoModeEnabled)
        {
            duoModeEnabled = true;
            EnableDuoMode();
        }
    }
}

void Tema::EnableDuoMode()
{


    
}

void Tema::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema::OnWindowResize(int width, int height)
{
}
