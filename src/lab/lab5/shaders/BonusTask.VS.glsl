#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_tex_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float appTime;


// Output
// TODO(student): Output values to fragment shader
out vec3 color;



void main()
{
    // TODO(student): Send output to fragment shader
    color = v_position;
   
    vec3 newPos = v_position;

    float randomNumber = v_position.x + v_position.y + v_position.z;
    
    newPos += sin(appTime * randomNumber * 10) / 20;


    // TODO(student): Compute gl_Position
     gl_Position = Projection * View * Model * vec4(newPos, 1.0);
}