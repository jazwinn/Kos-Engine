#ifndef ANICOM_H
#define ANICOM_H

#include "Component.h"

namespace ecs {


	class AnimationComponent : public Component {

	public:
		int m_frameNumber{};
		int m_framesPerSecond{};
		float m_frameTimer{};
		bool m_isAnimating{ true };

		REFLECTABLE(AnimationComponent, m_frameNumber, m_framesPerSecond, m_frameTimer, m_isAnimating)
	};

}




#endif ANICOM_H