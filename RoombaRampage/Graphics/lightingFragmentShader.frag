R"( #version 460 core

layout(location=0) in vec4 color;
//layout(location=1) in vec2 lightPos;
layout(location=2) in vec3 lightParams; //Intensity, Inner Radius, Outer Radius
layout(location=3) in float distance; 

layout (location=0) out vec4 fragColor;

void main() {


    // Calculate distance from light
    //float distance = length(gl_FragCoord.xy - lightPos);

    // Attenuation based on distance
    float attenuation = 1.0 - clamp(distance / lightParams.z, 0.0, 1.0);

    // Light effect
    vec3 lighting = (color.rgb * attenuation);
    fragColor = vec4(color.rgb * lighting, color.a);
}

)"