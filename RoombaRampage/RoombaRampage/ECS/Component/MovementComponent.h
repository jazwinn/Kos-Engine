#ifndef MOVECOM_H
#define MOVECOM_H

#include "Component.h"

namespace Ecs {

	class MovementComponent : public Component {

	public:
		float Speed{1};
		vector2::Vec2 Direction{0,0};
		bool Move = false;
	};


}



#endif MOVECOM_H
