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

uniform float explosionAnimTime;

// Output
layout(location = 0) out vec2 texture_coord;

void main()
{
    float explosionSpeed = 1.0f;

    for (int j = 0; j < 1; j++)
    {
        float pos_offset = float(j) * 3.0f;

        for (int i = 0; i < gl_in.length(); i+=3)
        {
            vec3 p1 = gl_in[i + 0].gl_Position.xyz;
            vec3 p2 = gl_in[i + 1].gl_Position.xyz;
            vec3 p3 = gl_in[i + 2].gl_Position.xyz;
 
            vec3 v12 = normalize(p2 - p1);
            vec3 v13 = normalize(p3 - p1);
 
            vec3 normal = cross(v12, v13);


            // First vertex
            {
                // Compute vertex coordinate in clip space
                gl_Position = Projection * View * (gl_in[i + 0].gl_Position + vec4(pos_offset, 4.0, 0.0, 0.0) + vec4(normal, 0.0) * explosionAnimTime * explosionSpeed);

                // Set texture coordinate
                texture_coord = v_texture_coord[i];

                // Emit vertex
                EmitVertex();
            }

            // Second vertex
            {
                // Compute vertex coordinate in clip space
                gl_Position = Projection * View * (gl_in[i + 1].gl_Position + vec4(pos_offset, 4.0, 0.0, 0.0) + vec4(normal, 0.0) * explosionAnimTime * explosionSpeed);

                // Set texture coordinate
                texture_coord = v_texture_coord[i];

                // Emit vertex
                EmitVertex();
            }

            // Third vertex
            {
                // Compute vertex coordinate in clip space
                gl_Position = Projection * View * (gl_in[i + 2].gl_Position + vec4(pos_offset, 4.0, 0.0, 0.0) + vec4(normal, 0.0) * explosionAnimTime * explosionSpeed);

                // Set texture coordinate
                texture_coord = v_texture_coord[i];

                // Emit vertex
                EmitVertex();
            }
        }

        // Go to the next object to render
        EndPrimitive();
    }
}
