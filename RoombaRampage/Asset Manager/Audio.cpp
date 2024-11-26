/******************************************************************/
/*!
\file      Audio.cpp
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 30, 2024
\brief     This file contains the implementation of the Audio class,
           which provides audio

This file implements a simple wrapper class FModAudio for handling 
audio playback using the FMOD sound system. It provides basic 
functionalities such as initializing the FMOD system, creating and 
playing sounds, controlling volume and panning, pausing and stopping 
playback, looping, and fading sound. Additionally, the file handles 
proper resource management by cleaning up the FMOD system and releasing 
resources when the audio operations are finished or when the object 
is destroyed.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../ECS/ECS.h"
#include "Audio.h"
//#include <fmod_errors.h>

namespace fmodaudio {
    FModAudio::FModAudio() : m_system(nullptr), m_sound(nullptr) {}

    FModAudio::~FModAudio() {
        m_StopAllSounds();
        m_Shutdown();
    }

    bool FModAudio::m_Init() {
        FMOD_RESULT result = FMOD::System_Create(&m_system);
        if (result != FMOD_OK) {
            return false;
        }

        result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
        if (result != FMOD_OK) {
            return false;
        }

        return true;
    }

    void FModAudio::m_Shutdown() {
        m_StopAllSounds();
        if (m_system) {
            m_system->release();
            m_system = nullptr;
        }
    }

    bool FModAudio::m_CreateSound(const char* soundFile) {
        FMOD_RESULT result = m_system->createSound(soundFile, FMOD_DEFAULT, nullptr, &m_sound);
        if (result != FMOD_OK) {
            return false;
        }
        return true;
    }

    bool FModAudio::m_PlaySound(const std::string& entityId) {
        if (!m_system || !m_sound) {
            return false;
        }

        FMOD::Channel* newChannel = nullptr;
        FMOD_RESULT result = m_system->playSound(m_sound, nullptr, false, &newChannel);
        if (result != FMOD_OK) {
            return false;
        }

        m_entityChannels[entityId] = newChannel;

        if (newChannel) {
            newChannel->setVolume(1.0f); 
        }

        return true;
    }

    void FModAudio::m_StopSound(const std::string& entityId) {
        auto it = m_entityChannels.find(entityId);
        if (it != m_entityChannels.end()) {
            it->second->stop();
            m_entityChannels.erase(it);
        }
    }

    void FModAudio::m_StopAllSounds() {
        for (auto& pair : m_entityChannels) {
            pair.second->stop();
        }
        m_entityChannels.clear();
    }

    void FModAudio::m_PauseAllSounds() {
        for (auto& pair : m_entityChannels) {
            FMOD::Channel* channel = pair.second;
            if (channel) {
                channel->setPaused(true);
            }
        }
    }

    bool FModAudio::m_UnpauseAllSounds() {
        for (auto& channelPair : m_entityChannels) {
            FMOD::Channel* channel = channelPair.second;
            if (channel) {
                bool isPaused = false;
                channel->getPaused(&isPaused);
                if (isPaused) {
                    channel->setPaused(false);
                }
            }
        }
        return true;
    }

    bool FModAudio::m_SetVolume(const std::string& entityId, float volume) {
        volume = std::clamp(volume, 0.0f, 1.0f);

        auto it = m_entityChannels.find(entityId);
        if (it != m_entityChannels.end()) {
            FMOD_RESULT result = it->second->setVolume(volume);
            if (result != FMOD_OK) {
                return false;
            }
            return true;
        }
        else {
            return false;
        }
    }

    bool FModAudio::m_SetPan(const std::string& entityId, float pan) {
        auto it = m_entityChannels.find(entityId);
        if (it != m_entityChannels.end()) {
            FMOD_RESULT result = it->second->setPan(pan);
            if (result != FMOD_OK) {
                return false;
            }
            return true;
        }
        else {
            return false;
        }
    }

    bool FModAudio::m_PauseSound(const std::string& entityId) {
        auto channelIt = m_entityChannels.find(entityId);
        if (channelIt != m_entityChannels.end()) {
            FMOD::Channel* channel = channelIt->second;
            bool isPaused = false;
            channel->getPaused(&isPaused);
            if (!isPaused) {
                channel->setPaused(true);
                return true;
            }
        }
        return false;
    }

    bool FModAudio::m_UnpauseSound(const std::string& entityId) {
        auto channelIt = m_entityChannels.find(entityId);
        if (channelIt != m_entityChannels.end()) {
            FMOD::Channel* channel = channelIt->second;
            bool isPaused = false;
            channel->getPaused(&isPaused);
            if (isPaused) {
                channel->setPaused(false);
                return true;
            }
        }
        return false;
    }


    bool FModAudio::m_IsPlaying(const std::string& entityId) {
        auto it = m_entityChannels.find(entityId);
        if (it != m_entityChannels.end()) {
            bool isPlaying = false;
            FMOD_RESULT result = it->second->isPlaying(&isPlaying);
            if (result != FMOD_OK) {
                return false;
            }
            return isPlaying;
        }
        else {
            return false;
        }
    }

    bool FModAudio::m_SetLooping(const std::string& entityId, bool loop) {
        auto it = m_entityChannels.find(entityId);
        if (it != m_entityChannels.end()) {
            FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
            FMOD_RESULT result = it->second->setMode(mode);
            if (result != FMOD_OK) {
                return false;
            }
            return true;
        }
        else {
            return false;
        }
    }

    bool FModAudio::m_FadeSound(const std::string& entityId, float targetVolume, float fadeDuration) {
        auto it = m_entityChannels.find(entityId);
        if (it == m_entityChannels.end()) {
            return false;
        }

        FMOD_RESULT result = it->second->setVolumeRamp(true);
        if (result != FMOD_OK) {
            return false;
        }

        float currentVolume;
        result = it->second->getVolume(&currentVolume);
        if (result != FMOD_OK) {
            return false;
        }

        int steps = 100;
        float volumeStep = (targetVolume - currentVolume) / steps;
        int delay = static_cast<int>(fadeDuration * 1000 / steps);  // Delay between steps (in milliseconds)

        for (int i = 0; i < steps; ++i) {
            currentVolume += volumeStep;
            it->second->setVolume(currentVolume);
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }

        it->second->setVolume(targetVolume);
        return true;
    }

    FMOD::Channel* FModAudio::m_GetChannelForEntity(const std::string& entityId) {
        auto it = m_entityChannels.find(entityId);
        if (it != m_entityChannels.end()) {
            return it->second;  
        }
        return nullptr;
    }





    // AudioManager Implementation (UPDATED 24/11/2024)
    AudioManager::AudioManager() {
    }

    AudioManager::~AudioManager() {
        for (auto& pair : m_soundMap) {
            FModAudio* sound = pair.second.get();
            if (sound) {
                sound->m_Shutdown();
            }
        }
    }

    void AudioManager::m_LoadAudio(const std::string& name, const std::string& path) {
        auto sound = std::make_unique<FModAudio>();

        if (sound->m_Init() && sound->m_CreateSound(path.c_str())) {
            m_soundMap[name] = std::move(sound);
        }
        else {
            //TODO Handle error (e.g., logging or notification)
            //std::cerr << "Failed to load audio: " << path << std::endl;
        }
    }

    void AudioManager::m_PlayAudioForEntity(const std::string& entityId, const std::string& name, float volume) {
        auto it = m_soundMap.find(name);
        if (it != m_soundMap.end()) {
            FModAudio* sound = it->second.get();
            if (sound->m_PlaySound(entityId)) {
                sound->m_SetVolume(entityId, volume);
            }
        }
        else {
            //TODO Handle error (e.g., logging or notification)
            //std::cerr << "Sound not found: " << name << std::endl;
        }
    }

    void AudioManager::m_StopAudioForEntity(const std::string& entityId, const std::string& name) {
        auto it = m_soundMap.find(name);
        if (it != m_soundMap.end()) {
            FModAudio* sound = it->second.get();
            sound->m_StopSound(entityId);
        }
        else {
            //TODO Handle error (e.g., logging or notification)
            //std::cerr << "Sound not found: " << name << std::endl;
        }
    }

    void AudioManager::m_PauseAudioForEntity(const std::string& entityId, const std::string& name) {
        auto& sound = m_soundMap[name];
        if (sound) {
            sound->m_PauseSound(entityId);
        }
    }

    void AudioManager::m_UnpauseAudioForEntity(const std::string& entityId, const std::string& name) {
        auto& sound = m_soundMap[name];
        if (sound) {
            sound->m_UnpauseSound(entityId);
        }
    }

    void AudioManager::m_SetVolumeForEntity(const std::string& entityId, const std::string& name, float volume) {
        auto it = m_soundMap.find(name);
        if (it != m_soundMap.end()) {
            FModAudio* sound = it->second.get();
            sound->m_SetVolume(entityId, volume);
        }
        else {
            //TODO Handle error (e.g., logging or notification)
            //std::cerr << "Sound not found: " << name << std::endl;
        }
    }

    void AudioManager::m_SetLoopingForEntity(const std::string& entityId, const std::string& name, bool loop) {
        auto it = m_soundMap.find(name);
        if (it != m_soundMap.end()) {
            auto& audio = it->second;
            FMOD::Channel* channel = audio->m_GetChannelForEntity(entityId);
            if (channel) {
                // Set the loop count: -1 for infinite loop, 0 for no loop, or a specific number for limited loops :))
                channel->setLoopCount(loop ? -1 : 0);
            }
        }
    }

    void AudioManager::m_SetPlayOnStartForEntity(const std::string& entityId, const std::string& audioName, bool playOnStart) {
        auto* ecs = ecs::ECS::m_GetInstance();
        auto* entityComponent = ecs->m_ECS_CombinedComponentPool[ecs::TYPEAUDIOCOMPONENT]->m_GetEntityComponent(std::stoi(entityId));

        if (entityComponent) {
            ecs::AudioComponent* ac = static_cast<ecs::AudioComponent*>(entityComponent);

            for (auto& audioFile : ac->m_AudioFiles) {
                if (audioFile.m_Name == audioName) {
                    audioFile.m_PlayOnStart = playOnStart;
                }
                else if (playOnStart) {
                    audioFile.m_PlayOnStart = false;
                }
            }
        }
    }


    bool AudioManager::m_IsPlayingForEntity(const std::string& entityId, const std::string& name) {
        auto it = m_soundMap.find(name);
        if (it != m_soundMap.end()) {
            FModAudio* sound = it->second.get();
            return sound->m_IsPlaying(entityId);
        }
        else {
            //TODO Handle error (e.g., logging or notification)
            //std::cerr << "Sound not found: " << name << std::endl;
            return false;
        }
    }

    void AudioManager::m_PauseAllSounds() {
        for (auto& soundPair : m_soundMap) {
            FModAudio* audio = soundPair.second.get();
            if (audio) {
                audio->m_PauseAllSounds();
            }
        }
    }

    void AudioManager::m_UnpauseAllSounds() {
        for (auto& soundPair : m_soundMap) {
            FModAudio* audio = soundPair.second.get();
            if (audio) {
                audio->m_UnpauseAllSounds();
            }
        }
    }


    void AudioManager::m_StopAllSounds() {
        for (auto& soundPair : m_soundMap) {
            FModAudio* audio = soundPair.second.get();
            if (audio) {
                audio->m_StopAllSounds();
            }
        }
    }
}