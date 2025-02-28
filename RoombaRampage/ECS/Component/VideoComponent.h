#pragma once

#include "Component.h"
#include "../Asset Manager/Video.h"

namespace ecs {

	class VideoComponent : public Component {

	public:

		bool play;

		std::string filename;
		
		bool pause;

		bool loop;

	public:

		std::shared_ptr<video::Video> m_video = nullptr;

		REFLECTABLE(VideoComponent, filename, pause, loop);

	};


}
