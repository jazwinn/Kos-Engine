#ifndef ANICOM_H
#define ANICOM_H

#include "Component.h"

namespace ecs {


	class AnimationComponent : public Component {

	public:
		float m_framesPerSecond;
		float m_frameTimer;
		bool m_isAnimating{ true };

	};

}




#endif ANICOM_H