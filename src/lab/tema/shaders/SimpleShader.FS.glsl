#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform float point_lights_count;
uniform vec3 point_light_positions[10];
uniform vec3 point_light_colors[10];
uniform vec3 spot_light_positions[10];
uniform vec3 spot_light_colors[10];
uniform vec3 spot_light_directions[10];
uniform float spot_light_angles[10];


uniform int spot_count;
uniform vec3 spot_positions[100];
uniform vec3 spot_directions[100];
uniform vec3 spot_colors[100];
uniform float spot_angles[100];



uniform vec3 eye_position;
uniform vec3 material_ka;
uniform vec3 material_kd;
uniform vec3 material_ks;
uniform int material_shininess;

// Output
layout(location = 0) out vec4 out_color;

vec3 ComputePhongIllumination(vec3 light_position)
{
    // TODO(student): Compute the diffuse component of the Lambert illumination model
    vec3 L = normalize(light_position - world_position);
    vec3 diffuse_component =  material_kd * max(dot(normalize(world_normal), L), 0);

    // TODO(student): Compute the specular component of the Phong illumination model
    int receivesLight = 0;
    // if (dot(world_normal, L) > 0)
    // {
        // receivesLight = 1;
    // }
    vec3 R = reflect (-L, normalize(world_normal));

    vec3 specular_component = vec3(0);

    if (length(diffuse_component) > 0)
    {
        receivesLight = 1;
    }

    specular_component = material_ks * receivesLight * pow(max(dot(normalize(eye_position), R), 0), material_shininess);

    // TODO(student): Compute the final illumination as the sum of the diffuse and specular components
    vec3 illumination = diffuse_component + specular_component;

    return illumination;
}

float ComputeDistanceAttenuation(vec3 light_position, vec3 point_position)
{
    // TODO(student): Compute the light attenuation factor based on the distance
    // between the position of the illuminated point and the position of the light source.
    float d = distance(light_position, point_position);

    return 1 / (pow(d, 2) + 1);
}

vec3 ComputePointLightSourcesIllumination()
{
    float strength = 5.0f; 
    vec3 lights_illumination = vec3(0);

    for (int i=0;i<10;i++) {
        vec3 light_position = point_light_positions[i];
        vec3 light_color = point_light_colors[i];

        vec3 light_illumination = ComputePhongIllumination(light_position);
        float illumination_attenuation = ComputeDistanceAttenuation(light_position, world_position);

        // TODO(student): Add to the illumination of all light sources the result
        // of multiplying the illumination of the light source from the current iteration
        // with the attenuation of the illumination and the color of the illumination.
        lights_illumination += strength * light_illumination * illumination_attenuation * light_color;
    }

    return lights_illumination;
}

vec3 ComputeSpotLightSourcesIllumination()
{
    float strength = 10.0f;
    vec3 lights_illumination = vec3(0);

    for (int i=0;i<spot_count;i++) {
        vec3 light_position = spot_positions[i];
        vec3 light_color = spot_colors[i];
        vec3 light_direction = spot_directions[i];
        float cut_off_angle = spot_angles[i];

        vec3 L = normalize(light_position-world_position);
        float cos_theta_angle = dot(-L, light_direction);
        float cos_phi_angle = cos(cut_off_angle);
        if (cos_theta_angle > cos_phi_angle)
        {
            vec3 light_illumination = ComputePhongIllumination(light_position);
            float illumination_attenuation = ComputeDistanceAttenuation(light_position, world_position);

            // TODO(student): Compute the attenuation factor specific to the spot light source
            float spot_linear_att_factor = (cos_theta_angle - cos_phi_angle) / (1.0f - cos_phi_angle);
            float quadratic_spot_light_att_factor = pow(spot_linear_att_factor, 2);


            // TODO(student): Add to the illumination of all light sources the result
            // of multiplying the illumination of the light source from the current iteration
            // with the attenuation of the illumination, the attenuation factor specific
            // to the light spot source and the color of the illumination.
            lights_illumination += strength * light_illumination * illumination_attenuation * quadratic_spot_light_att_factor * light_color;
        }
    }

    return lights_illumination;
}

vec3 ComputeAmbientComponent()
{
    vec3 global_ambient_color = vec3(0.25f);

    // TODO(student): Compute the ambient component of global illumination
    vec3 ambient_component = material_ka * global_ambient_color;

    return ambient_component;
}

void main()
{
    vec3 illumination = ComputePointLightSourcesIllumination()
        + ComputeSpotLightSourcesIllumination ()
        + ComputeAmbientComponent();

    out_color = vec4 (illumination, 1);
}
