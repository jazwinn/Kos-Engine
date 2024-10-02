#ifndef ANICOM_H
#define ANICOM_H

#include "Component.h"

namespace ecs {


	class AnimationComponent : public Component {

	public:

		int m_frameNumber;
		float m_frameTimer;
		bool m_isAnimating{ true };

	};

}




#endif ANICOM_H