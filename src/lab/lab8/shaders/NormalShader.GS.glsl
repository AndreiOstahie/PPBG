#version 430

// Input and output topologies
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

// Input
layout(location = 0) in vec3 normal[];

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;

// TODO(student): Declare other uniforms here

// Output
layout(location = 0) out vec3 color;


void EmitPoint(vec3 position, vec3 c)
{
    color = c;
    gl_Position = Projection * View * vec4(position, 1);
    EmitVertex();
}

void main()
{
    for (int i = 0; i < gl_in.length(); i++)
    {
        // Compute vertex coordinate in clip space
        vec3 pos = gl_in[i].gl_Position.xyz;

        // Emit vertex
        EmitPoint(pos, vec3(0, 0, 1));

        // Compute endpoint of the normal line
        vec3 normalEndpoint = pos + normal[i] * 3;

        // Emit normal line endpoint
        EmitPoint(normalEndpoint, vec3(0, 0, 1));

        EndPrimitive();
    }
}
