#include "Audio.h"
#include <iostream>
#include <fmod_errors.h>

namespace fmodaudio {
    FModAudio::FModAudio() : m_system_(nullptr), m_sound_(nullptr), m_channel_(nullptr) {}

    FModAudio::~FModAudio() {
        m_stopSound();
        m_shutdown();
    }

    bool FModAudio::m_init() {
        FMOD_RESULT result = FMOD::System_Create(&m_system_);
        if (result != FMOD_OK) {
            std::cerr << "FMOD error: Failed to create system: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        result = m_system_->init(32, FMOD_INIT_NORMAL, nullptr);
        if (result != FMOD_OK) {
            std::cerr << "FMOD error: Failed to initialize system: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        return true;
    }

    void FModAudio::m_shutdown() {
        if (m_channel_) {
            m_channel_->stop();
            m_channel_ = nullptr;
        }
        if (m_sound_) {
            m_sound_->release();
            m_sound_ = nullptr;
        }
        if (m_system_) {
            m_system_->release();
            m_system_ = nullptr;
        }
    }

    bool FModAudio::m_createSound(const char* soundFile) {
        if (!m_system_) {
            std::cerr << "FMOD error: System is not initialized." << std::endl;
            return false;
        }

        FMOD_RESULT result = m_system_->createSound(soundFile, FMOD_DEFAULT, nullptr, &m_sound_);
        if (result != FMOD_OK) {
            std::cerr << "FMOD error: Failed to create sound: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }

        return true;
    }

    bool FModAudio::m_playSound() {
        if (!m_sound_) {
            std::cerr << "FMOD error: Sound is not created." << std::endl;
            return false;
        }

        if (!m_channel_) {
            FMOD_RESULT result = m_system_->playSound(m_sound_, nullptr, false, &m_channel_);
            if (result != FMOD_OK) {
                std::cerr << "FMOD error: Failed to play sound: " << FMOD_ErrorString(result) << std::endl;
                return false;
            }
            std::cout << "Sound played\n";
        }
        else {
            bool isPlaying;
            m_channel_->isPlaying(&isPlaying);
            if (!isPlaying) {
                FMOD_RESULT result = m_system_->playSound(m_sound_, nullptr, false, &m_channel_);
                if (result != FMOD_OK) {
                    std::cerr << "FMOD error: Failed to replay sound: " << FMOD_ErrorString(result) << std::endl;
                    return false;
                }
                std::cout << "Sound replayed\n";
            }
        }

        return true;
    }

    void FModAudio::m_stopSound() {
        if (m_channel_) {
            m_channel_->stop();
            m_channel_ = nullptr;
        }
    }

    // Volume control (0.0 = silent, 1.0 = full volume)
    bool FModAudio::m_setVolume(float volume) {
        if (!m_channel_) {
            std::cerr << "FMOD error: Channel is not initialized." << std::endl;
            return false;
        }
        FMOD_RESULT result = m_channel_->setVolume(volume);
        if (result != FMOD_OK) {
            std::cerr << "FMOD error: Failed to set volume: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }
        return true;
    }

    // Panning control (-1.0 = full left, 1.0 = full right, 0.0 = center)
    bool FModAudio::m_setPan(float pan) {
        if (!m_channel_) {
            std::cerr << "FMOD error: Channel is not initialized." << std::endl;
            return false;
        }
        FMOD_RESULT result = m_channel_->setPan(pan);
        if (result != FMOD_OK) {
            std::cerr << "FMOD error: Failed to set panning: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }
        return true;
    }

}
