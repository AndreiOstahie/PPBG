#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 playerPos;
uniform mat4 playerModelMat;
uniform float curveFactor;

// Output value to fragment shader
out vec3 world_position;
out vec3 world_normal;

void main()
{

    world_position = (Model * vec4(v_position, 1)).xyz;
    world_normal = normalize( mat3(Model) * v_normal );
    vec3 world_playerPos = (playerModelMat * vec4(playerPos, 1)).xyz;

   // Position after curvature effect
   // vec3 new_position = v_position;
   // new_position.y = v_position.y - pow(length(normalize(world_playerPos - world_position)), 2) * curveFactor;


    gl_Position = Projection * View * Model * vec4(v_position, 1.0);


/*
    world_position = (Model * vec4(v_position, 1)).xyz;
    world_normal = normalize(mat3(Model) * v_normal);

    vec3 world_playerPos = (playerModelMat * vec4(playerPos, 1)).xyz;
    float distance = length(world_position - world_playerPos);
    float curvature = pow(distance, 2.0) * curveFactor;
    world_position.y = world_position.y - curvature;

    gl_Position = Projection * View * vec4(world_position, 1.0);
*/
}
