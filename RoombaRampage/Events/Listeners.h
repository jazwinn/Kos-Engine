/******************************************************************/
/*!
\file      Listeners.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds any functions that are to be used as a callback

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "BaseMessage.h"
#include "../Application/Application.h"
#include "../Assets/AssetManager.h"
#include <iostream>

namespace messaging {
	void SoundPlayed(const Message& givenMsg);
}