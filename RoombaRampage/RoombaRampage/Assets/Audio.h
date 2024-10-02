#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "fmod.hpp"
#include "fmod_errors.h"


class FModAudio {
public:
    FModAudio();
    ~FModAudio();

    bool init();
    void shutdown();
    bool createSound(const char* soundFile);
    bool playSound();
    void stopSound();
    bool pauseSound();
    bool fadeSound(float targetVolume, float fadeDuration);
    bool setLooping(bool loop);
    bool setVolume(float volume);
    bool setPan(float pan);
    bool isPlaying();

    FMOD::System* system_;
    FMOD::Sound* sound_;
    FMOD::Channel* channel_;
};

#endif // AUDIO_MANAGER_H