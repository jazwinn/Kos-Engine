#pragma once

#include "Component.h"
#include "../Asset Manager/Video.h"

namespace ecs {

	class VideoComponent : public Component {

	public:

		bool play = false;

		std::string filename;
		
		bool pause = false;

		bool loop = true;

		float layer = 0.0f;

	public:

		REFLECTABLE(VideoComponent, filename, pause, loop, layer);

	};


}
