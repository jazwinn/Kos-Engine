R"( #version 460 core

layout(location=0) in vec4 color;
layout(location=1) in vec2 lightPos;
layout(location=2) in vec3 lightParams; //Intensity, Inner Radius, Outer Radius
//layout(location=4) in vec2 fragPos;
layout(location=5) in vec2 lightDirection;
layout (location=0) out vec4 fragColor;

void main() 
{
	 // Calculate distance and direction from light to fragment
    float distance = length(lightDirection);

    // Calculate attenuation (e.g., inverse square falloff)
    float attenuation = clamp(1.0 - (distance / lightParams.z), 0.0, 1.0);

    // Combine light color with attenuation
    vec3 lighting = vec3(color.rgb);

   lighting = lighting;

    fragColor = vec4(lighting,attenuation * lightParams.x); // Preserve texture alpha
}

)"