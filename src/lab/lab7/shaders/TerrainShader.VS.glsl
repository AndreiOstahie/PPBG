#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int texture_unit;


// Output
out vec2 texture_coord;


void main()
{
    // TODO(student): Pass v_texture_coord as output to fragment shader
    texture_coord = v_texture_coord;


    vec4 heightTex = texture(texture_2, texture_coord);
    
    vec3 height = vec3(heightTex.x, heightTex.y, heightTex.z);

    vec3 newPos = vec3(v_position.x, v_position.y + height.x * 10, v_position.z);

    gl_Position = Projection * View * Model * vec4(newPos, 1.0);
}
