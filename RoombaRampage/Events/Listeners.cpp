#include "Listeners.h"

namespace messaging {
	void SoundPlayed(const Message& givenMsg){
		assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
		assetManager->m_audioContainer[0]->m_playSound();
		std::cout << "Sound has been played! Messaging system called from " << givenMsg.m_GetData() << std::endl;
	}
}