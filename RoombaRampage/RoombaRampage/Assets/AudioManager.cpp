#include "AudioManager.h"
#include <iostream>
#include <fmod_errors.h>

FModAudio::FModAudio() : system_(nullptr), sound_(nullptr), channel_(nullptr) {}

FModAudio::~FModAudio() {
    shutdown();
}

bool FModAudio::init() {
    FMOD_RESULT result = FMOD::System_Create(&system_);
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: Failed to create system: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }

    result = system_->init(32, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: Failed to initialize system: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }

    return true;
}

void FModAudio::shutdown() {
    if (channel_) {
        channel_->stop();
        channel_ = nullptr;
    }
    if (sound_) {
        sound_->release();
        sound_ = nullptr;
    }
    if (system_) {
        system_->release();
        system_ = nullptr;
    }
}

bool FModAudio::createSound(const char* soundFile) {
    if (!system_) {
        std::cerr << "FMOD error: System is not initialized." << std::endl;
        return false;
    }

    FMOD_RESULT result = system_->createSound(soundFile, FMOD_DEFAULT, nullptr, &sound_);
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: Failed to create sound: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }

    return true;
}

bool FModAudio::playSound() {
    if (!sound_) {
        std::cerr << "FMOD error: Sound is not created." << std::endl;
        return false;
    }

    if (!channel_) {
        FMOD_RESULT result = system_->playSound(sound_, nullptr, false, &channel_);
        if (result != FMOD_OK) {
            std::cerr << "FMOD error: Failed to play sound: " << FMOD_ErrorString(result) << std::endl;
            return false;
        }
        std::cout << "Sound played\n";
    }
    else {
        bool isPlaying;
        channel_->isPlaying(&isPlaying);
        if (!isPlaying) {
            FMOD_RESULT result = system_->playSound(sound_, nullptr, false, &channel_);
            if (result != FMOD_OK) {
                std::cerr << "FMOD error: Failed to replay sound: " << FMOD_ErrorString(result) << std::endl;
                return false;
            }
            std::cout << "Sound replayed\n";
        }
    }

    return true;
}

void FModAudio::stopSound() {
    if (channel_) {
        channel_->stop();
        channel_ = nullptr;
    }
}

// Volume control (0.0 = silent, 1.0 = full volume)
bool FModAudio::setVolume(float volume) {
    if (!channel_) {
        std::cerr << "FMOD error: Channel is not initialized." << std::endl;
        return false;
    }
    FMOD_RESULT result = channel_->setVolume(volume);
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: Failed to set volume: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }
    return true;
}

// Panning control (-1.0 = full left, 1.0 = full right, 0.0 = center)
bool FModAudio::setPan(float pan) {
    if (!channel_) {
        std::cerr << "FMOD error: Channel is not initialized." << std::endl;
        return false;
    }
    FMOD_RESULT result = channel_->setPan(pan);
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: Failed to set panning: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }
    return true;
}
