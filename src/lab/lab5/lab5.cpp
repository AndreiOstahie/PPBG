#include "lab/lab5/lab5.h"

#include <vector>
#include <string>
#include <iostream>

#include "components/transform.h"

using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab5::Lab5()
{
}


Lab5::~Lab5()
{
}


void Lab5::Init()
{
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
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

    CreateShader("LabShader",
        "src/lab/lab5/shaders/LabShader.VS.glsl",
        "src/lab/lab5/shaders/LabShader.FS.glsl"
    );

    CreateShader("LastTask",
        "src/lab/lab5/shaders/LastTask.VS.glsl",
        "src/lab/lab5/shaders/LastTask.FS.glsl"
    );

    CreateShader("BonusTask",
        "src/lab/lab5/shaders/BonusTask.VS.glsl",
        "src/lab/lab5/shaders/BonusTask.FS.glsl"
    );
}

void Lab5::CreateShader(const char* name, const char* vertex_shader_path, const char* fragment_shader_path)
{
    // TODO(student): Create and compile the vertex shader object
    unsigned int vertex_shader_id = 0;
    vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    const char *vertex_shader_source = GetShaderContent(vertex_shader_path);
    glShaderSource(vertex_shader_id, 1, &vertex_shader_source, 0);
    glCompileShader(vertex_shader_id);

    CheckShaderCompilationError(vertex_shader_id);

    // TODO(student): Create and compile the fragment shader object
    unsigned int fragment_shader_id = 0;
    fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragment_shader_source = GetShaderContent(fragment_shader_path);
    glShaderSource(fragment_shader_id, 1, &fragment_shader_source, 0);
    glCompileShader(fragment_shader_id);

    CheckShaderCompilationError(fragment_shader_id);

    // TODO(student): Create the program, attach the two shader
    // objects and link them.
    unsigned int program_id = 0;
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    CheckShadersLinkingError(program_id);

    shaders[name] = new Shader(name);
    shaders[name]->program = program_id;
}

Mesh* Lab5::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices)
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


void Lab5::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Lab5::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();

    viewport_space = transform2D::ViewportSpace(0, 0, resolution.x, resolution.y);

    glViewport(0, 0, viewport_space.width, viewport_space.height);

    {
        glm::mat4 model = glm::mat4(1);
        model *= transform3D::Translate(glm::vec3(-2, 0.5f, 0));
        model *= transform3D::RotateOX(glm::radians(60.0f));
        model *= transform3D::RotateOY(glm::radians(60.0f));
        RenderMesh(meshes["sphere"], shaders["BonusTask"], model, GetSceneCamera(), viewport_space);
    }

    {
        glm::mat4 model = glm::mat4(1);
        model *= transform3D::Translate(glm::vec3(0, 1, 0));
        model *= transform3D::RotateOY(glm::radians(45.0f));
        RenderMesh(meshes["cube"], shaders["LabShader"], model, GetSceneCamera(), viewport_space);
    }

    {
        glm::mat4 model = glm::mat4(1);
        model *= transform3D::Translate(glm::vec3(2, 0.5f, 0));
        model *= transform3D::RotateOX(glm::radians(60.0f));
        RenderMesh(meshes["cube"], shaders["LastTask"], model, GetSceneCamera(), viewport_space);
    }
}


void Lab5::FrameEnd()
{
    DrawCoordinateSystem();
}


void Lab5::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 & model,
    const gfxc::Camera *camera, const transform2D::ViewportSpace &viewport_space)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // TODO(student): Get shader location for uniform mat4 "Model"
    int modelLocation = glGetUniformLocation(shader->program, "Model");

    // TODO(student): Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

    // TODO(student): Get shader location for uniform mat4 "View"
    int viewLocation = glGetUniformLocation(shader->program, "View");

    // TODO(student): Set shader uniform "View" to viewMatrix
    glm::mat4 view = transform3D::View(
        camera->m_transform->GetWorldPosition(),
        camera->m_transform->GetLocalOZVector(),
        camera->m_transform->GetLocalOXVector(),
        camera->m_transform->GetLocalOYVector()
    );
    glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

    // TODO(student): Get shader location for uniform mat4 "Projection"
    int projectionLocation = glGetUniformLocation(shader->program, "Projection");

    // TODO(student): Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projection = transform3D::Perspective(
        glm::radians(60.0f), (float)viewport_space.width / viewport_space.height, 0.1f, 100.0f
    );
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

    // TODO(student): Send the application time, obtained by
    // calling Engine::GetElapsedTime(), in the form of a
    // uniform type attribute to the shader
    int timeLocation = glGetUniformLocation(shader->program, "appTime");
    glUniform1f(timeLocation, Engine::GetElapsedTime());

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

const char* Lab5::GetShaderContent(const char* shader_file_path)
{
    FILE* fp;
    long lSize;
    char* buffer;

    fp = fopen(shader_file_path, "rb");
    if (!fp) perror(shader_file_path), exit(1);

    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    /* allocate memory for entire content */
    buffer = (char*)calloc(1, lSize + 1);
    if (!buffer) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

    /* copy the file into the buffer */
    if (1 != fread(buffer, lSize, 1, fp))
        fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);

    fclose(fp);

    return buffer;
}

void Lab5::CheckShaderCompilationError(unsigned int shader_id)
{
    GLint isCompiled = 0;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetShaderInfoLog(shader_id, maxLength, &maxLength, &infoLog[0]);

        for (auto c : infoLog) {
            printf("%c", c);
        }

        // We don't need the shader anymore.
        glDeleteShader(shader_id);
    }
}

void Lab5::CheckShadersLinkingError(unsigned int program_id)
{
    GLint isLinked = 0;
    glGetProgramiv(program_id, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program_id, maxLength, &maxLength, &infoLog[0]);

        for (auto c : infoLog) {
            printf("%c", c);
        }

        // We don't need the program anymore.
        glDeleteProgram(program_id);
    }
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab5::OnInputUpdate(float deltaTime, int mods)
{
    // Add key press event
}


void Lab5::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Lab5::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab5::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab5::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab5::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab5::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Lab5::OnWindowResize(int width, int height)
{
}
