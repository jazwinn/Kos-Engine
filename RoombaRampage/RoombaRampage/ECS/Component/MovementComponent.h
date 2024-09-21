#ifndef MOVECOM_H
#define MOVECOM_H

#include "Component.h"

namespace Ecs {

	class MovementComponent : public Component {

	public:
		float Speed{};
		Vector2::Vec2 Direction{0,1};//facing north
	};


}



#endif MOVECOM_H
