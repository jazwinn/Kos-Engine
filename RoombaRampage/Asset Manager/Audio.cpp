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

#include "Audio.h"
//#include <fmod_errors.h>

namespace fmodaudio {

    FModAudio::FModAudio() : m_system(nullptr), m_sound(nullptr), m_channel(nullptr) {}

    FModAudio::~FModAudio() {
        m_StopSound();
        m_Shutdown();
    }

    bool FModAudio::m_Init() {
        FMOD_RESULT result = FMOD::System_Create(&m_system);
        if (result != FMOD_OK) {
            //std::cerr << "FMOD error: Failed to create system: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
        if (result != FMOD_OK) {
            //std::cerr << "FMOD error: Failed to initialize system: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        return true;
    }

    void FModAudio::m_Shutdown() {
        if (m_channel) {
            m_channel->stop();
            m_channel = nullptr;
        }
        if (m_sound) {
            m_sound->release();
            m_sound = nullptr;
        }
        if (m_system) {
            m_system->release();
            m_system = nullptr;
        }
    }

    bool FModAudio::m_CreateSound(const char* filePath) {
        FMOD_RESULT result = m_system->createSound(filePath, FMOD_DEFAULT, nullptr, &m_sound);
        return result == FMOD_OK;
    }

    bool FModAudio::m_PlaySound() {
        if (!m_system || !m_sound) {
            std::cerr << "FMOD system or sound not initialized.\n";
            return false;
        }

        FMOD::Channel* newChannel = nullptr;
        FMOD_RESULT result = m_system->playSound(m_sound, nullptr, false, &newChannel);

        if (result != FMOD_OK) {
            std::cerr << "Failed to play sound.\n";
            return false;
        }

        if (newChannel) {
            m_channel = newChannel;
            m_channel->setVolume(1.0f);
        }

        return true;
    }



    void FModAudio::m_StopSound() {
        if (m_channel) {
            std::cout << "Stopping sound on channel." << std::endl;
            m_channel->stop();
            m_channel = nullptr;
        }
        else {
            std::cerr << "No active channel to stop sound." << std::endl;
        }
    }


    bool FModAudio::m_SetVolume(float volume) {
        if (!m_channel) {
            //std::cerr << "FMOD error: Channel is not initialized." << std::endl;
            return false;
        }
        FMOD_RESULT result = m_channel->setVolume(volume);
        if (result != FMOD_OK) {
            //std::cerr << "FMOD error: Failed to set volume: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }
        return true;
    }

    bool FModAudio::m_SetPan(float pan) {
        if (!m_channel) {
            //std::cerr << "FMOD error: Channel is not initialized." << std::endl;
            return false;
        }
        FMOD_RESULT result = m_channel->setPan(pan);
        if (result != FMOD_OK) {
            //std::cerr << "FMOD error: Failed to set panning: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }
        return true;
    }

    bool FModAudio::m_PauseSound() {
        if (!m_channel) {
            //std::cerr << "FMOD error: Channel is not initialized." << std::endl;
            return false;
        }
        bool paused;
        m_channel->getPaused(&paused);
        FMOD_RESULT result = m_channel->setPaused(!paused);
        if (result != FMOD_OK) {
            //std::cerr << "FMOD error: Failed to toggle pause: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }
        return true;
    }

    bool FModAudio::m_IsPlaying() {
        if (!m_channel) return false;

        bool isPlaying = false;
        FMOD_RESULT result = m_channel->isPlaying(&isPlaying);
        if (result != FMOD_OK) {
            //std::cerr << "FMOD error: Failed to get playing state: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }
        return isPlaying;
    }

    bool FModAudio::m_SetLooping(bool loop) {
        if (!m_sound) {
            //std::cerr << "FMOD error: Sound is not created." << std::endl;
            return false;
        }

        FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
        FMOD_RESULT result = m_sound->setMode(mode);
        if (result != FMOD_OK) {
            //std::cerr << "FMOD error: Failed to set looping: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        return true;
    }

    bool FModAudio::m_FadeSound(float targetVolume, float fadeDuration) {
        if (!m_channel) {
            //std::cerr << "FMOD error: Channel is not initialized." << std::endl;
            return false;
        }

        FMOD_RESULT result = m_channel->setVolumeRamp(true);
        if (result != FMOD_OK) {
            //std::cerr << "FMOD error: Failed to enable volume ramping: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        float currentVolume;
        result = m_channel->getVolume(&currentVolume);
        if (result != FMOD_OK) {
            //std::cerr << "FMOD error: Failed to get current volume: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        int steps = 100;
        float volumeStep = (targetVolume - currentVolume) / steps;
        int delay = static_cast<int>(fadeDuration * 1000 / steps);  // Delay between steps (in milliseconds)

        for (int i = 0; i < steps; ++i) {
            currentVolume += volumeStep;
            m_channel->setVolume(currentVolume);

            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }

        m_channel->setVolume(targetVolume);

        return true;
    }


    // AudioManager Implementation (UPDATED 21/11/2024)
    AudioManager::AudioManager() = default;

    AudioManager::~AudioManager() {
        for (auto& [name, sound] : m_soundMap) {
            sound->m_Shutdown();
        }
    }

    void AudioManager::m_LoadAudio(const std::string& name, const std::string& path) {
        if (m_soundMap.find(name) == m_soundMap.end()) {
            auto sound = std::make_unique<FModAudio>();
            if (sound->m_Init() && sound->m_CreateSound(path.c_str())) {
                m_soundMap[name] = std::move(sound);
            }
            else {
                std::cerr << "Failed to load audio: " << name << " from path: " << path << "\n";
            }
        }
    }

    void AudioManager::m_PlayAudio(const std::string& name) {
        if (m_soundMap.find(name) != m_soundMap.end()) {
            m_soundMap[name]->m_PlaySound();
        }
        else {
            std::cerr << "Audio not found: " << name << "\n";
        }
    }

    void AudioManager::m_StopAudio(const std::string& name) {
        auto it = m_soundMap.find(name);
        if (it != m_soundMap.end()) {
            it->second->m_StopSound();
            std::cout << "Stopped sound with key: " << name << std::endl;
        }
        else {
            std::cerr << "Error: Sound not found for key: " << name << std::endl;
        }
    }

    void AudioManager::m_SetVolume(const std::string& name, float volume) {
        if (m_soundMap.find(name) != m_soundMap.end()) {
            m_soundMap[name]->m_SetVolume(volume);
        }
    }

    void AudioManager::m_SetLooping(const std::string& name, bool loop) {
        if (m_soundMap.find(name) != m_soundMap.end()) {
            m_soundMap[name]->m_SetLooping(loop);
        }
    }

    bool AudioManager::m_IsPlaying(const std::string& name) {
        if (m_soundMap.find(name) != m_soundMap.end()) {
            return m_soundMap[name]->m_IsPlaying();
        }
        return false;
    }

}