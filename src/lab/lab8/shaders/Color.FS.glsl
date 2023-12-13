#version 430

// Input
layout(location = 0) in vec3 color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
	// TODO(student): Use the color information
	// received from the geometry shader
    out_color = vec4(color, 0);
}
