/******************************************************************/
/*!
\file      Audio.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 30, 2024
\brief     This file contains the declaration of the FModAudio class,
           which acts as a simple wrapper around the FMOD audio system
           for handling audio playback and control.

This header file provides the declaration of the FModAudio class, which
utilizes the FMOD sound system to enable various audio functionalities.
These include initializing the FMOD system, creating and playing audio
files, controlling audio parameters such as volume, panning, and looping,
as well as handling sound playback with pause, stop, and fade features.
The class ensures proper resource management by cleaning up FMOD objects
and releasing resources when no longer in use.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "fmod.hpp"
//#include "fmod_errors.h"
#include <thread>
#include "../Config/pch.h"
namespace fmodaudio {

    /******************************************************************/
    /*!
    \class   FModAudio
    \brief   A class that provides an interface to the FMOD audio system.
             It manages audio initialization, sound creation, playback,
             and control over various sound parameters such as volume,
             pan, and looping.
    */
    /******************************************************************/
    class FModAudio {
    public:
        FModAudio();
        ~FModAudio();

        bool m_Init();
        void m_Shutdown();

        bool m_CreateSound(const char* soundFile);
        bool m_PlaySound(const std::string& entityId);
        void m_StopSound(const std::string& entityId); 
        void m_StopAllSounds();
        void m_PauseAllSounds();
        bool m_UnpauseAllSounds();
        bool m_PauseSound(const std::string& entityId); 
        bool m_UnpauseSound(const std::string& entityId);
        bool m_FadeSound(const std::string& entityId, float targetVolume, float fadeDuration); 
        bool m_SetLooping(const std::string& entityId, bool loop);  
        bool m_SetVolume(const std::string& entityId, float volume); 
        bool m_SetPan(const std::string& entityId, float pan);  
        bool m_IsPlaying(const std::string& entityId);  
        FMOD::Channel* m_GetChannelForEntity(const std::string& entityId);

    private:
        FMOD::System* m_system;    /*!< FMOD system object to manage sound playback. */
        FMOD::Sound* m_sound;      /*!< FMOD sound object to represent the loaded audio. */
        std::unordered_map<std::string, FMOD::Channel*> m_entityChannels;  /*!< Map of entity ID to its playing FMOD channel. */
    };



    class AudioManager {
    public:
        AudioManager();
        ~AudioManager();

        void m_LoadAudio(const std::string& name, const std::string& path);
        void m_PlayAudioForEntity(const std::string& entityId, const std::string& name, float volume);
        void m_StopAudioForEntity(const std::string& entityId, const std::string& name);
        void m_PauseAudioForEntity(const std::string& entityId, const std::string& name);
        void m_UnpauseAudioForEntity(const std::string& entityId, const std::string& name);
        void m_SetVolumeForEntity(const std::string& entityId, const std::string& name, float volume);
        void m_SetLoopingForEntity(const std::string& entityId, const std::string& name, bool loop);
        void m_SetPlayOnStartForEntity(const std::string& entityId, const std::string& audioName, bool playOnStart);
        bool m_IsPlayingForEntity(const std::string& entityId, const std::string& name);
        void m_PauseAllSounds();
        void m_UnpauseAllSounds();
        void m_StopAllSounds();

        std::unordered_map<std::string, std::unique_ptr<FModAudio>>& getSoundMap() {
            return m_soundMap;
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<FModAudio>> m_soundMap;
    };
}
#endif // AUDIO_MANAGER_H
