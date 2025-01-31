R"( #version 460 core

layout(location=0) in vec4 color;
layout(location=1) in vec2 lightPos;
layout(location=2) in vec3 lightParams; //Intensity, Inner Radius, Outer Radius

layout(location=5) in vec2 lightDirection;
layout (location=0) out vec4 fragColor;

void main() 
{
	 // Compute the distance from light source
    float distance = length(lightDirection);

    // Initialize attenuation
    float attenuation = 0.0;

    // Inner and outer radius from lightParams
    float innerRadius = lightParams.y;
    float outerRadius = lightParams.z;

    // If inside inner radius, full brightness
    if (distance < innerRadius)
    {
        attenuation = 1.0;
    }
    // If between inner and outer radius, apply smooth falloff
    else if (distance < outerRadius)
    {
        float t = (distance - innerRadius) / (outerRadius - innerRadius);
        attenuation = pow(clamp(1.0 - t, 0.0, 1.0), 2.0); // Quadratic falloff
    }

    // Compute final lighting color
    vec3 lighting = color.rgb;
    fragColor = vec4(lighting, attenuation * lightParams.x);
}

)"