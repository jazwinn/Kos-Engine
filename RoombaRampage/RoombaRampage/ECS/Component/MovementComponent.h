#ifndef MOVECOM_H
#define MOVECOM_H

#include "Component.h"
#include "../Dependencies/rapidjson/document.h"

namespace Ecs {

	class MovementComponent : public Component {

	public:
		float Speed{};
		Vector2::Vec2 Direction{0,1};//facing north

		MovementComponent() : Speed(0), Direction(0,1){}

	

	};
}



#endif MOVECOM_H
