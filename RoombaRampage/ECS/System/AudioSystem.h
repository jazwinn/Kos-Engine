#ifndef AUDIOSYS_H
#define AUDIOSYS_H

#include "../ECS.h"
#include "System.h"

namespace ecs {

    class AudioSystem : public ISystem {
    public:
        /******************************************************************/
        /*!
        \fn      void AudioSystem::m_RegisterSystem(EntityID ID)
        \brief   Registers an entity with the audio system.
        \param   ID - The ID of the entity to be registered.
        \details Adds the entity to the audio system, allowing it to be updated for audio playback.
        */
        /******************************************************************/
        void m_RegisterSystem(EntityID ID) override;

        /******************************************************************/
        /*!
        \fn      void AudioSystem::m_DeregisterSystem(EntityID ID)
        \brief   Deregisters an entity from the audio system.
        \param   ID - The ID of the entity to be deregistered.
        \details Removes the entity from the audio system, stopping its updates for audio playback.
        */
        /******************************************************************/
        void m_DeregisterSystem(EntityID ID) override;

        /******************************************************************/
        /*!
        \fn      void AudioSystem::m_Init()
        \brief   Initializes the audio system.
        \details Prepares necessary resources and sets up the audio system for use.
        */
        /******************************************************************/
        void m_Init() override;

        /******************************************************************/
        /*!
        \fn      void AudioSystem::m_Update(const std::string& scene)
        \brief   Updates the audio system.
        \param   scene - The name of the scene to be updated.
        \details Performs updates on all entities registered to the audio system for audio playback.
        */
        /******************************************************************/
        void m_Update(const std::string& scene) override;


    private:
        /******************************************************************/
        /*!
        \var     std::vector<AudioComponent*> m_vecAudioComponentPtr
        \brief   Stores pointers to AudioComponent instances for registered entities.
        \details Used to manage and update audio playback for entities in the system.
        */
        /******************************************************************/
        std::vector<AudioComponent*> m_vecAudioComponentPtr;

        /******************************************************************/
        /*!
        \var     std::vector<TransformComponent*> m_vecTransformComponentPtr
        \brief   Stores pointers to TransformComponent instances for registered entities.
        \details Used to manage position-related data for audio sources in the system.
        */
        /******************************************************************/
        std::vector<TransformComponent*> m_vecTransformComponentPtr;
    };
}

#endif // AUDIOSYS_H
