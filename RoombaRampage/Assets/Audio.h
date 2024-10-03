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
#include "fmod_errors.h"
#include <thread>

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
        /******************************************************************/
        /*!
        \fn     FModAudio::FModAudio()
        \brief  Constructor that initializes member variables to null.
        */
        /******************************************************************/
        FModAudio();

        /******************************************************************/
        /*!
        \fn     FModAudio::~FModAudio()
        \brief  Destructor that stops any sound playing and releases
                system resources.
        */
        /******************************************************************/
        ~FModAudio();

        /******************************************************************/
        /*!
        \fn     bool FModAudio::m_Init()
        \brief  Initializes the FMOD system for audio playback.
        \return Returns true if the system is initialized successfully,
                otherwise false and logs an error.
        */
        /******************************************************************/
        bool m_Init();

        /******************************************************************/
        /*!
        \fn     void FModAudio::m_Shutdown()
        \brief  Shuts down the FMOD system, releasing any allocated resources.
        */
        /******************************************************************/
        void m_Shutdown();

        /******************************************************************/
        /*!
        \fn     bool FModAudio::m_CreateSound(const char* soundFile)
        \brief  Loads a sound file for playback.
        \param  soundFile - The path to the sound file to be loaded.
        \return Returns true if the sound is created successfully,
                otherwise false and logs an error.
        */
        /******************************************************************/
        bool m_CreateSound(const char* soundFile);

        /******************************************************************/
        /*!
        \fn     bool FModAudio::m_PlaySound()
        \brief  Plays the loaded sound file. If the sound is already playing,
                it will be restarted if it's not currently playing.
        \return Returns true if the sound is played successfully,
                otherwise false and logs an error.
        */
        /******************************************************************/
        bool m_PlaySound();

        /******************************************************************/
        /*!
        \fn     void FModAudio::m_StopSound()
        \brief  Stops the currently playing sound and releases the channel.
        */
        /******************************************************************/
        void m_StopSound();

        /******************************************************************/
        /*!
        \fn     bool FModAudio::m_PauseSound()
        \brief  Toggles the pause state of the currently playing sound.
        \return Returns true if the pause state is successfully toggled,
                otherwise false and logs an error.
        */
        /******************************************************************/
        bool m_PauseSound();

        /******************************************************************/
        /*!
        \fn     bool FModAudio::m_FadeSound(float targetVolume, float fadeDuration)
        \brief  Fades the sound volume smoothly to the target volume over
                the specified duration.
        \param  targetVolume - The final volume after fading (0.0 to 1.0).
        \param  fadeDuration - The duration of the fade effect in seconds.
        \return Returns true if the fade is successful, otherwise false and logs an error.
        */
        /******************************************************************/
        bool m_FadeSound(float targetVolume, float fadeDuration);

        /******************************************************************/
        /*!
        \fn     bool FModAudio::m_SetLooping(bool loop)
        \brief  Sets whether the sound should loop after playback.
        \param  loop - A boolean flag, true to enable looping, false to disable.
        \return Returns true if the looping mode is successfully set,
                otherwise false and logs an error.
        */
        /******************************************************************/
        bool m_SetLooping(bool loop);

        /******************************************************************/
        /*!
        \fn     bool FModAudio::m_SetVolume(float volume)
        \brief  Sets the volume of the currently playing sound.
        \param  volume - The desired volume level (0.0 to 1.0).
        \return Returns true if the volume is set successfully,
                otherwise false and logs an error.
        */
        /******************************************************************/
        bool m_SetVolume(float volume);

        /******************************************************************/
        /*!
        \fn     bool FModAudio::m_SetPan(float pan)
        \brief  Sets the stereo panning of the currently playing sound.
        \param  pan - The desired panning value (-1.0 for left, 1.0 for right, 0.0 for center).
        \return Returns true if the pan is set successfully, otherwise false and logs an error.
        */
        /******************************************************************/
        bool m_SetPan(float pan);

        /******************************************************************/
        /*!
        \fn     bool FModAudio::m_IsPlaying()
        \brief  Checks if the sound is currently playing.
        \return Returns true if the sound is playing, otherwise false.
        */
        /******************************************************************/
        bool m_IsPlaying();

    private:
        FMOD::System* m_system;    /*!< FMOD system object to manage sound playback. */
        FMOD::Sound* m_sound;      /*!< FMOD sound object to represent the loaded audio. */
        FMOD::Channel* m_channel;  /*!< FMOD channel object for controlling sound playback. */
    };
}
#endif // AUDIO_MANAGER_H
