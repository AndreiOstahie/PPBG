#version 430

// Input and output topologies
// TODO(student): First, generate a curve (via line strip),
// then a rotation/translation surface (via triangle strip)
layout(lines) in;
layout(triangle_strip, max_vertices = 256) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p0, control_p1, control_p2, control_p3;
uniform int no_of_instances;

// TODO(student): Declare any other uniforms here
uniform int no_of_generated_points;
uniform int surface_type;
uniform float max_translate;
uniform float max_rotate;

// Input
in int instance[2];

vec3 rotateY(vec3 point, float u)
{
    float x = point.x * cos(u) - point.z *sin(u);
    float z = point.x * sin(u) + point.z *cos(u);
    return vec3(x, point.y, z);
}

vec3 translateX(vec3 point, float t)
{
    return vec3(point.x + t, point.y, point.z);
}

void main()
{
    const int SURFACE_TYPE_TRANSLATION = 0;
    const int SURFACE_TYPE_ROTATION = 1;

    /*
    gl_Position = Projection * View * vec4(control_p0, 1);   EmitVertex();
    gl_Position = Projection * View * vec4(control_p1, 1);   EmitVertex();
    gl_Position = Projection * View * vec4(control_p2, 1);   EmitVertex();
    gl_Position = Projection * View * vec4(control_p3, 1);   EmitVertex();
    EndPrimitive();
    */

    // TODO(student): Rather than emitting vertices for the control
    // points, you must emit vertices that approximate the curve itself.


    if (surface_type == SURFACE_TYPE_TRANSLATION)
    {
        for (int k = 0; k < no_of_instances; k++)
        {
            float instanceTranslate1 = k * max_translate / float(no_of_instances);
            float instanceTranslate2 = (k + 1) * max_translate / float(no_of_instances);

            for (int i = 0; i <= no_of_generated_points; i++)
            {
                vec3 p01 = translateX(control_p0, instanceTranslate1);
                vec3 p11 = translateX(control_p1, instanceTranslate1);
                vec3 p21 = translateX(control_p2, instanceTranslate1);
                vec3 p31 = translateX(control_p3, instanceTranslate1);

                vec3 p02 = translateX(control_p0, instanceTranslate2);
                vec3 p12 = translateX(control_p1, instanceTranslate2);
                vec3 p22 = translateX(control_p2, instanceTranslate2);
                vec3 p32 = translateX(control_p3, instanceTranslate2);

                float t = float(i) / float(no_of_generated_points);
                vec3 bezierPoint = mix(mix(mix(p01, p11, t), mix(p11, p21, t), t), mix(mix(p11, p21, t), mix(p21, p31, t), t), t);
                gl_Position = Projection * View * vec4(bezierPoint, 1);
                EmitVertex();
           
                bezierPoint = mix(mix(mix(p02, p12, t), mix(p12, p22, t), t), mix(mix(p12, p22, t), mix(p22, p32, t), t), t);
                gl_Position = Projection * View * vec4(bezierPoint, 1);
                EmitVertex();
            }

            EndPrimitive();
        }
    }
    else if (surface_type == SURFACE_TYPE_ROTATION)
    {
        for (int k = 0; k < no_of_instances; k++)
        {
            float instanceTranslate1 = k * max_rotate / float(no_of_instances);
            float instanceTranslate2 = (k + 1) * max_rotate / float(no_of_instances);

            for (int i = 0; i <= no_of_generated_points; i++)
            {
                vec3 p01 = rotateY(control_p0, instanceTranslate1);
                vec3 p11 = rotateY(control_p1, instanceTranslate1);
                vec3 p21 = rotateY(control_p2, instanceTranslate1);
                vec3 p31 = rotateY(control_p3, instanceTranslate1);

                vec3 p02 = rotateY(control_p0, instanceTranslate2);
                vec3 p12 = rotateY(control_p1, instanceTranslate2);
                vec3 p22 = rotateY(control_p2, instanceTranslate2);
                vec3 p32 = rotateY(control_p3, instanceTranslate2);

                float t = float(i) / float(no_of_generated_points);
                vec3 bezierPoint = mix(mix(mix(p01, p11, t), mix(p11, p21, t), t), mix(mix(p11, p21, t), mix(p21, p31, t), t), t);
                gl_Position = Projection * View * vec4(bezierPoint, 1);
                EmitVertex();

                bezierPoint = mix(mix(mix(p02, p12, t), mix(p12, p22, t), t), mix(mix(p12, p22, t), mix(p22, p32, t), t), t);
                gl_Position = Projection * View * vec4(bezierPoint, 1);
                EmitVertex();
            }

            EndPrimitive();
        }
    }
    

    

    
}




/*
for (int k = 0; k < no_of_instances; k++)
    {
        // float instanceTranslate = max_translate * float(k);
        float instanceTranslate = k * max_translate / float(no_of_instances);

        for (int i = 0; i <= no_of_generated_points; i++)
        {
            float t = float(i) / float(no_of_generated_points);
            vec3 bezierPoint = mix(mix(mix(control_p0, control_p1, t), mix(control_p1, control_p2, t), t), mix(mix(control_p1, control_p2, t), mix(control_p2, control_p3, t), t), t);
            gl_Position = Projection * View * vec4(bezierPoint, 1);
            EmitVertex();

            bezierPoint = translateX(bezierPoint, instanceTranslate);
            gl_Position = Projection * View * vec4(bezierPoint, 1);
            EmitVertex();
        }

        EndPrimitive();
    }
    */