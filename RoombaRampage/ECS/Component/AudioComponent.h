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


    class AudioComponent : public Component {

    public:
        std::string m_AudioFile{};
        float m_Volume{ 1.0f };
        bool m_Loop{ false };
        bool m_PlayOnStart{ false };

        REFLECTABLE(AudioComponent, m_AudioFile, m_Volume, m_Loop, m_PlayOnStart)
    };

}

#endif AUDIOCOM_H