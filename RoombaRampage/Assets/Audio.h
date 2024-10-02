#ifndef AUDIO_H
#define AUDIO_H

#include "fmod.hpp"
#include "fmod_errors.h"

namespace fmodaudio{
    class FModAudio {
    public:
        FModAudio();
        ~FModAudio();

        bool m_init();
        void m_shutdown();
        bool m_createSound(const char* soundFile);
        bool m_playSound();
        void m_stopSound();
        bool m_setVolume(float volume);
        bool m_setPan(float pan);

        FMOD::System* m_system_;
        FMOD::Sound* m_sound_;
        FMOD::Channel* m_channel_;
    };
}


#endif AUDIO_H