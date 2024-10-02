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
#include "Listeners.h"


namespace messaging {
	/**
	 \brief		this function is a callback function that will be attached to a message type that will call the playsound function
	 **/
	void SoundPlayed(const Message& givenMsg){
		assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
		assetManager->m_audioContainer[0]->m_PlaySound();
		std::cout << "Sound has been played! Messaging system called from " << givenMsg.m_GetData() << std::endl;
	}
}