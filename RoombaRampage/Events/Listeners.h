#pragma once
#include "BaseMessage.h"
#include "../Application/Application.h"
#include "../Assets/AssetManager.h"
#include <iostream>

namespace messaging {
	void SoundPlayed(const Message& givenMsg);
}