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
        std::cerr << "Failed to load sound file: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }
    system_->init(32, FMOD_INIT_NORMAL, nullptr);
    return true;
}

void FModAudio::shutdown() {
    if (system_) {
        if (channel_) {
            channel_->stop();
            channel_ = nullptr;
        }
        if (sound_) {
            sound_->release();
            sound_ = nullptr;
        }
        system_->release();
        system_ = nullptr;
    }
}

bool FModAudio::createSound(const char* soundFile) {
    FMOD_RESULT result = system_->createSound(soundFile, FMOD_DEFAULT, nullptr, &sound_);
    if (result != FMOD_OK) {
        std::cerr << FMOD_ErrorString(result) << std::endl;
        return false;
    }
}

bool FModAudio::playSound() {
    bool isplaying{false};
    channel_->isPlaying(&isplaying);
    if (!isplaying)
    {
        std::cout << "played\n";
        FMOD_RESULT result = system_->playSound(sound_, nullptr, false, &channel_);
        if (result != FMOD_OK) {
            std::cerr << FMOD_ErrorString(result) << std::endl;
            return false;
        }
    }
    
    return true;
}

void FModAudio::stopSound() {
    if (channel_) {
        channel_->stop();
        channel_->setPaused(true); // Pause the channel instead of releasing it
        channel_ = nullptr;
    }
}