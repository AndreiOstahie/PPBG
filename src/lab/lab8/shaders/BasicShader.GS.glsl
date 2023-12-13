#version 430

// Input and output topologies
layout(triangles) in;
layout(triangle_strip, max_vertices = 170) out;

// Input
layout(location = 1) in vec2 v_texture_coord[];

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
// TODO(student): Declare other uniforms here

// Output
layout(location = 0) out vec2 texture_coord;

void main()
{
    for (int i = 0; i < gl_in.length(); i++)
    {
        // Compute vertex coordinate in clip space
        gl_Position = Projection * View * (gl_in[i].gl_Position);

        // Set texture coordinate
        texture_coord = v_texture_coord[i];

        // Emit vertex
        EmitVertex();
    }

    // Go to the next object to render
    EndPrimitive();
}
