#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "fmod.hpp"
#include "fmod_errors.h"

namespace fmodaudio {
    class FModAudio {
    public:
        FModAudio();
        ~FModAudio();

        bool m_Init();
        void m_Shutdown();
        bool m_CreateSound(const char* soundFile);
        bool m_PlaySound();
        void m_StopSound();
        bool m_PauseSound();
        bool m_FadeSound(float targetVolume, float fadeDuration);
        bool m_SetLooping(bool loop);
        bool m_SetVolume(float volume);
        bool m_SetPan(float pan);
        bool m_IsPlaying();

        FMOD::System* m_system;
        FMOD::Sound* m_sound;
        FMOD::Channel* m_channel;
    };
}
#endif // AUDIO_MANAGER_H