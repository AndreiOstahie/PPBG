#version 330

// Input
in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;

// TODO(student): Declare various other uniforms
uniform int texture_unit;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture() function.
    out_color = vec4(1);
    if (texture_unit == 0)
    {
        vec4 color = texture(texture_1, texture_coord);
        out_color = color;
    }
    else
    {
        vec4 color = texture(texture_2, texture_coord);
        out_color = color;
    }
    
    // TODO(student): Use the "discard" directive to terminate execution
    // based on the value of the alpha channel
    if(out_color.a < 0.5) {
        discard;
    }
}
