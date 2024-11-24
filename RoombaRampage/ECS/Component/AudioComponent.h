#pragma once
/******************************************************************/
/*!
\file      AudioComponent.h
\author    Clarence Boey, c.boey, 2301547
\par       c.boey@digipen.edu
\date      Nov 19, 2024
\brief     This file contains the audio data


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef AUDIOCOM_H
#define AUDIOCOM_H

#include "Component.h"
#include <string>

namespace ecs {

    struct AudioFile {
        std::string m_Name;
        std::string m_FilePath;
        float m_Volume{ 1.0f };
        bool m_Loop{ false };
        bool m_PlayOnStart{ false };
        bool m_HasPlayed{ false };

        REFLECTABLE(AudioFile, m_FilePath, m_Volume, m_Loop, m_PlayOnStart)
    };

    class AudioComponent : public Component {
    public:
        std::string m_EntityId;
        std::vector<AudioFile> m_AudioFiles;

        REFLECTABLE(AudioComponent, m_AudioFiles)
    };

}

#endif AUDIOCOM_H