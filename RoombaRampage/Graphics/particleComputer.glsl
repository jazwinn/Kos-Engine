#version 430 core
layout(local_size_x = 256) in; // Work-group size (256 threads per group)

// SSBO binding
struct ParticleData {
    int particleAmount;
    float lifespan;
    vec2 position;
    vec2 velocity;
    vec2 acceleration;
    vec2 scale;
    vec4 color;
    float rotation;
    float randomFactor;
    int stripCount;
    int frameNumber;
    int layer;
};

layout(std430, binding = 0) buffer Particles {
    ParticleData particles[];
};

uniform float deltaTime;
//uniform vec3 gravity = vec3(0.0, -9.8, 0.0);

void main() {
    uint id = gl_GlobalInvocationID.x;
    
    if (id >= particles.length()) return;

    // Update velocity (apply gravity)
    // particles[id].velocity.xyz += gravity * deltaTime;

    // Update position
    particles[id].position.xyz += particles[id].velocity.xyz * deltaTime;

    // Reduce lifetime
    particles[id].position.w -= deltaTime;

    // Respawn if dead
    if (particles[id].position.w <= 0.0) {
        particles[id].position.xyz = vec3(0.0, 0.0, 0.0);  // Respawn at origin
        particles[id].velocity.xyz = vec3(rand(), rand(), rand()) * 2.0 - 1.0; // Random velocity
        particles[id].position.w = 5.0;  // Reset lifetime
    }
}
