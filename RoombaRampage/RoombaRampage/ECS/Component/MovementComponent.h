#ifndef MOVECOM_H
#define MOVECOM_H

#include "Component.h"

namespace ecs {

	class MovementComponent : public Component {

	public:
		float m_Speed{1};
		vector2::Vec2 m_Direction{0,0};
		bool m_Move = false;
	};


}



#endif MOVECOM_H
