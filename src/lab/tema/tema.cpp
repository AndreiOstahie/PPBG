#include "lab/tema/tema.h"
#include "lab/tema/enemy.h"

#include <vector>
#include <string>
#include <iostream>

#include "glm/gtx/string_cast.hpp"
#include <glm/gtc/random.hpp>

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
        spot_light_positions[9] = glm::vec3(1, 1, 1);

        for (int i = 0; i < 10; i++) {
            spot_light_directions[i] = glm::vec3(0, -1, 0);
            spot_light_angles[i] = glm::radians(10.0f + rand() % 50);

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


    // Variables init
    lightpost_color = glm::vec3(1, 0, 0);
    ground_color = glm::vec3(0, 1, 0);
    player_color = glm::vec3(0, 0, 1);

    playerRotY = 0.0f;
    playerPos = glm::vec3(0, 0, 0);
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
    spot_count = lightposts_per_row * lightposts_per_row * 2 + 1;

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
    spot_positions[spot_index] = playerPos + glm::vec3(0, 2.0f, 0);
    spot_directions[spot_index] = glm::vec3(0, 0, -1);
    spot_angles[spot_index] = 60.0f;
    spot_colors[spot_index] = glm::vec3(1, 1, 1);

    // Generate random rotations for each spotlight
    for (int i = 0; i < spot_count - 1; i++)
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

    // Set camera position to follow player
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(
        playerPos + cameraOffset,
        glm::quatLookAt(-glm::normalize(cameraOffset), glm::vec3(0, 1, 0))
    );


    // Update the rotation timer
    timeSinceLastRotation += deltaTimeSeconds;

    if (timeSinceLastRotation >= timeBetweenRotations)
    {
        // Reset timer
        timeSinceLastRotation = 0.0f;

        // Generate random rotations for each spotlight
        for (int i = 0; i < spot_count - 1; i++)
        {
            float angle_x = glm::linearRand(0.0f, spot_rotation_max_cap); // Random rotation around X axis
            float angle_z = glm::linearRand(0.0f, spot_rotation_max_cap); // Random rotation around Z axis

            spot_rotations[i] = glm::vec2(angle_x, angle_z);
        }
    }
    else
    {
        for (int i = 0; i < spot_count - 1; i++)
        {
            // Apply rotations
            glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(spot_rotations[i].x * deltaTimeSeconds * spot_rotation_speed), glm::vec3(1, 0, 0))
                                * glm::rotate(glm::mat4(1), glm::radians(spot_rotations[i].y * deltaTimeSeconds * spot_rotation_speed), glm::vec3(0, 0, 1));

            // Update spot light direction
            spot_directions[i] = glm::mat3(rotation) * spot_directions[i];
        }

        // Rotate spotlights
        //float rotationSpeed = 30.0f;  // Adjust this value as needed
        //glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(rotationSpeed * deltaTimeSeconds), glm::vec3(1, 0, 0));
        //spot_directions[i] = glm::mat3(rotation) * spot_directions[i];
    }


    // Render ground
    {
        glm::mat4 model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0, 0.1f, 0));
        model = glm::scale(model, glm::vec3(groundScale));
        RenderSimpleMesh(meshes["ground"], shaders["SimpleShader"], model, ground_color);
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
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, player_color);
        }

        // Head
        {
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, playerPos); // translate limb to player's current pos
            model = glm::rotate(model, RADIANS(playerRotY), glm::vec3(0, 1, 0)); // player rotation
            model = glm::translate(model, glm::vec3(0, 2.2f, 0)); // translate limb relative to player's body
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, player_color);
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
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, player_color);
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
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, player_color);
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
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, player_color);
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
            RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, player_color);
        }

        // Spot Light
        {
            spot_positions[spot_count - 1] = playerPos + glm::vec3(0, 3.0f, 0);
            glm::mat4 model = glm::mat4(1);
            model = glm::translate(model, spot_positions[spot_count - 1]);
            model = glm::scale(model, glm::vec3(0.1f));
            // RenderMesh(meshes["sphere"], shaders["Simple"], model);
        }

        // Point Light
        {
            point_light_positions[9] = playerPos + glm::vec3(0, 2.5f, 1);
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
        RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, lightpost_color);

        model = glm::mat4(1);
        model = glm::translate(model, lightpost_positions[i]);
        model = glm::rotate(model, RADIANS(30), glm::vec3(0, 1, 0));
        model = glm::translate(model, glm::vec3(0, 5.0f, 0));
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 3.0f));
        RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, lightpost_color);


        // Spot lights
        model = glm::mat4(1);
        model = glm::translate(model, spot_positions[i * 2]);
        model = glm::scale(model, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["Simple"], model);

        model = glm::mat4(1);
        model = glm::translate(model, spot_positions[i * 2 + 1]);
        model = glm::scale(model, glm::vec3(0.1f));
        RenderMesh(meshes["sphere"], shaders["Simple"], model);
    }

    // Enemies
    {
        for (int i = 0; i < enemyCount; i++)
        {
            // Enemies follow player
            enemies[i].Follow(playerPos, deltaTimeSeconds);
            enemies[i].Animate(deltaTimeSeconds);


            // Render enemies
            {
                // Body
                {
                    glm::mat4 model = glm::mat4(1);
                    model = glm::translate(model, enemies[i].GetPosition()); // translate limb to enemy's current pos
                    model = glm::rotate(model, RADIANS(enemies[i].GetRotation().y), glm::vec3(0, 1, 0)); // enemy rotation
                    model = glm::translate(model, glm::vec3(0, 1.55f, 0)); // translate limb relative to enemy's body
                    model = glm::scale(model, glm::vec3(0.5f, 1.0f, 0.25f));
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, enemies[i].GetColor());
                }

                // Head
                {
                    glm::mat4 model = glm::mat4(1);
                    model = glm::translate(model, enemies[i].GetPosition()); // translate limb to enemy's current pos
                    model = glm::rotate(model, RADIANS(enemies[i].GetRotation().y), glm::vec3(0, 1, 0)); // enemy rotation
                    model = glm::translate(model, glm::vec3(0, 2.2f, 0)); // translate limb relative to enemy's body
                    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, enemies[i].GetColor());
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
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, enemies[i].GetColor());
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
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, enemies[i].GetColor());
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
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, enemies[i].GetColor());
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
                    RenderSimpleMesh(meshes["cube"], shaders["SimpleShader"], model, enemies[i].GetColor());
                }
            }
        }
    }


    // Set up and render lights from lab 6
    /*
    * for (int i = 0; i < spot_count; i++)
    {
        glm::mat4 rotation = glm::rotate(glm::mat4(1), glm::radians(50 * deltaTimeSeconds), glm::vec3(1, 0, 0));
        spot_directions[i] = glm::mat3(rotation) * spot_directions[i];
    }

         angle += glm::radians(6.0f) * deltaTimeSeconds;
        for (int i = 0; i < 9; i++) {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0), angle + i * glm::radians(360.0f) / 9, glm::vec3(0, 1, 0));

            point_light_positions[i] = glm::vec3(glm::mat3(rotation) * glm::vec3(5, 1.5 + sin(Engine::GetElapsedTime() + i / 2.0f), 0));
            spot_light_positions[i] = glm::vec3(glm::mat3(rotation) * glm::vec3(3, 1.5 + sin(Engine::GetElapsedTime() + i / 2.0f), 0));
        }

        // Render the point lights in the scene
        for (int i = 0; i < 9; i++)
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
    */
}


void Tema::Attack()
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



void Tema::FrameEnd()
{
    DrawCoordinateSystem();
}

void Tema::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& model, const glm::vec3& object_color)
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


    glm::vec3 eye_position = GetSceneCamera()->m_transform->GetWorldPosition();

    // TODO(student): Set eye position (camera position) uniform
    int eye_position_location = glGetUniformLocation(shader->program, "eye_position");
    glUniform3fv(eye_position_location, 1, glm::value_ptr(eye_position));


    int playerPos_location = glGetUniformLocation(shader->program, "playerPos");
    glUniform3fv(playerPos_location, 1, glm::value_ptr(playerPos));

    int curveFactor_location = glGetUniformLocation(shader->program, "curveFactor");
    glUniform1f(curveFactor_location, curveFactor);


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
        glm::vec3& light_direction = spot_light_directions[9];
        float& angle = spot_light_angles[9];
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
            spot_directions[spot_count - 1] = glm::vec3(0, 0, -1);
        }
        // Forward-left
        if (movement.x < 0 && movement.z < 0)
        {
            playerRotY = 180.0f + 45.0f;
            spot_directions[spot_count - 1] = glm::vec3(-1, 0, -1);
        }
        // Forward-right
        if (movement.x > 0 && movement.z < 0)
        {
            playerRotY = 180.0f - 45.0f;
            spot_directions[spot_count - 1] = glm::vec3(1, 0, -1);
        }
        // Left
        if (movement.x < 0 && movement.z == 0)
        {
            playerRotY = -90.0f;
            spot_directions[spot_count - 1] = glm::vec3(-1, 0, 0);
        }
        // Right
        if (movement.x > 0 && movement.z == 0)
        {
            playerRotY = 90.0f;
            spot_directions[spot_count - 1] = glm::vec3(1, 0, 0);
        }
        // Back
        if (movement.x == 0 && movement.z > 0)
        {
            playerRotY = 0.0f;
            spot_directions[spot_count - 1] = glm::vec3(0, 0, 1);
        }
        // Back-left
        if (movement.x < 0 && movement.z > 0)
        {
            playerRotY = -45.0f;
            spot_directions[spot_count - 1] = glm::vec3(-1, 0, 1);
        }
        // Back-right
        if (movement.x > 0 && movement.z > 0)
        {
            playerRotY = 45.0f;
            spot_directions[spot_count - 1] = glm::vec3(1, 0, 1);
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
}


void Tema::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F) {
        controlled_light_source_index = (controlled_light_source_index + 1) % 2;
    }

    if (key == GLFW_KEY_SPACE)
    {
        Attack();
    }
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
