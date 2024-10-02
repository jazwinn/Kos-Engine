#include "Audio.h"
#include <chrono>
#include <iostream>
#include <fmod_errors.h>

FModAudio::FModAudio() : system_(nullptr), sound_(nullptr), channel_(nullptr) {}

FModAudio::~FModAudio() {
    stopSound();
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

bool FModAudio::pauseSound() {
    if (!channel_) {
        std::cerr << "FMOD error: Channel is not initialized." << std::endl;
        return false;
    }
    bool paused;
    channel_->getPaused(&paused);
    FMOD_RESULT result = channel_->setPaused(!paused);
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: Failed to toggle pause: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }
    return true;
}

bool FModAudio::isPlaying() {
    if (!channel_) return false;

    bool isPlaying = false;
    FMOD_RESULT result = channel_->isPlaying(&isPlaying);
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: Failed to get playing state: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }
    return isPlaying;
}

bool FModAudio::setLooping(bool loop) {
    if (!sound_) {
        std::cerr << "FMOD error: Sound is not created." << std::endl;
        return false;
    }

    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    FMOD_RESULT result = sound_->setMode(mode);
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: Failed to set looping: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }

    return true;
}

#include <chrono>

// Function to smoothly fade sound to a target volume over a specified duration
bool FModAudio::fadeSound(float targetVolume, float fadeDuration) {
    if (!channel_) {
        std::cerr << "FMOD error: Channel is not initialized." << std::endl;
        return false;
    }

    // Enable volume ramping for smooth transition
    FMOD_RESULT result = channel_->setVolumeRamp(true);
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: Failed to enable volume ramping: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }

    // Get the current volume
    float currentVolume;
    result = channel_->getVolume(&currentVolume);
    if (result != FMOD_OK) {
        std::cerr << "FMOD error: Failed to get current volume: " << FMOD_ErrorString(result) << std::endl;
        return false;
    }

    // Calculate the time step based on fade duration
    int steps = 100; // Number of small steps for smoother fade
    float volumeStep = (targetVolume - currentVolume) / steps;
    int delay = static_cast<int>(fadeDuration * 1000 / steps);  // Delay between steps (in milliseconds)

    // Gradually adjust the volume in small steps
    for (int i = 0; i < steps; ++i) {
        currentVolume += volumeStep;
        channel_->setVolume(currentVolume);

        // Pause for a bit to allow smooth fading
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }

    // Ensure the final volume is set to the exact target
    channel_->setVolume(targetVolume);

    return true;
}


