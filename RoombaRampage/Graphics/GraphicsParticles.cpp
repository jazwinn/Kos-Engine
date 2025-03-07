#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"
#include <random>


namespace graphicpipe
{



    // Function to generate a random float in range [minVal, maxVal]
    float randomInRange(float minVal, float maxVal) 
    {
        static std::random_device rd;  // Seed
        static std::mt19937 gen(rd()); // Mersenne Twister PRNG
        std::uniform_real_distribution<float> dist(minVal, maxVal);
        return dist(gen);
    }

    void PrintParticleData(const ParticleData& particle, int index)
    {
        std::cout << "Particle [" << index << "]\n"
            << "  Life Span: " << particle.m_lifeSpan << "\n"
            << "  Position: (" << particle.m_position.x << ", " << particle.m_position.y << ")\n"
            << "  Velocity: (" << particle.m_velocity.x << ", " << particle.m_velocity.y << ")\n"
            << "  Acceleration: (" << particle.m_acceleration.x << ", " << particle.m_acceleration.y << ")\n"
            << "  Scale: (" << particle.m_scale.x << ", " << particle.m_scale.y << ")\n"
            << "  Color: (" << particle.m_color.r << ", " << particle.m_color.g << ", " << particle.m_color.b << ", " << particle.m_color.a << ")\n"
            << "  Rotation: " << particle.m_rotation << "\n"
            //<< "  Cone Rotation: " << particle.m_coneRotation << "\n"
           // << "  Cone Angle: " << particle.m_coneAngle << "\n"
           // << "  Random Factor: " << particle.m_randomFactor << "\n"
            << "  Texture ID: " << particle.m_textureID << "\n"
            << "  Strip Count: " << particle.m_stripCount << "\n"
            << "  Frame Number: " << particle.m_frameNumber << "\n"
            << "  Layer: " << particle.m_layer << "\n"
            << "  Is Active: " << particle.m_isActive << "\n"
            << "-----------------------------------\n";
    }

    void GraphicsPipe::m_spawnParticles() 
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_particleSSBO);

        ParticleData* particles = (ParticleData*)glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE);

        if (particles) 
        {
            for (size_t i = 0; i < m_emitterData.size(); i++)
            {
                if (m_emitterData[i].m_noOfParticles >= 0)
                {
                    int particlesProcessed = 0;
                    const float radian = 3.1415f / 180.f;
                    for (int j = 0; j < MAX_PARTICLES; j++)
                    {
                        if (particles[j].m_isActive < 1.f)
                        {
                           // float randomValue = randomInRange(-m_emitterData[i].m_randomFactor / 2.f, m_emitterData[i].m_randomFactor / 2.f);
                            particles[j].m_lifeSpan = m_emitterData[i].m_lifeSpan;
                            particles[j].m_position = m_emitterData[i].m_position;
                            particles[j].m_scale = m_emitterData[i].m_scale;
                            particles[j].m_color = m_emitterData[i].m_color;
                            particles[j].m_rotation = m_emitterData[i].m_rotation;
                           
                            float newAngle = m_emitterData[i].m_coneRotation + randomInRange(-m_emitterData[i].m_coneAngle / 2.f, m_emitterData[i].m_coneAngle / 2.f);

                            newAngle *= radian;

                            glm::vec2 newVelocity = { m_emitterData[i].m_velocity.x * cos(newAngle) + m_emitterData[i].m_velocity.y * sin(newAngle),
                                                    -m_emitterData[i].m_velocity.x * sin(newAngle) + m_emitterData[i].m_velocity.y * cos(newAngle) };
                            
                            particles[j].m_initialEmissionAngle = newAngle;

                            particles[j].m_velocity = newVelocity; //* randomValue;
                            particles[j].m_acceleration = m_emitterData[i].m_acceleration; //* randomValue;

                            particles[j].m_textureID = m_emitterData[i].m_textureID;
                            particles[j].m_stripCount = m_emitterData[i].m_stripCount;
                            particles[j].m_frameNumber = m_emitterData[i].m_frameNumber;
                            particles[j].m_layer = m_emitterData[i].m_layer;
                            particles[j].m_isActive = 2.f;
                            particles[j].m_friction = m_emitterData[i].m_friction;
                            particlesProcessed++;
                            if (particlesProcessed > m_emitterData[i].m_noOfParticles)
                                break;

                        }
                    }
                }
               
            }
        }

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void GraphicsPipe::m_updateParticles()
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_particleSSBO);
        //std::vector<ParticleData> debugData(MAX_PARTICLES);
        //glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(ParticleData) * MAX_PARTICLES, debugData.data());
        glUseProgram(m_particleComputerShaderProgram);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_particleSSBO);

        // Pass uniforms
        glUniform1f(glGetUniformLocation(m_particleComputerShaderProgram, "deltaTime"), Helper::Helpers::GetInstance()->m_fixedDeltaTime);

        // Dispatch Compute Shader
        int workGroups = (MAX_PARTICLES + 255) / 256;
        glDispatchCompute(workGroups, 1, 1);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, 0);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_particleSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    
}