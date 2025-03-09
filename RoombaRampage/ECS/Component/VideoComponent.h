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

		int layer = 0;

	public:

		REFLECTABLE(VideoComponent, filename, pause, loop, layer);

	};


}
