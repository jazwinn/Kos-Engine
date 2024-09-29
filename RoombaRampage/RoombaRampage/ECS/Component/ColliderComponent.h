#ifndef BOXCOM_H
#define BOXCOM_H

#include "../Math/vector2.h"
#include "Component.h"
#include "../Physics/Physics.h"

namespace Ecs {


	class ColliderComponent : public Component {

	public:
		
		Vector2::Vec2 Size = {1,1};

		Vector2::Vec2 OffSet{};

		unsigned int Layer{};

		bool drawDebug{true};

		Physics::EntityType type = Physics::EntityType::Rectangle;

		float radius{};
	};

}




#endif BOXCOM_H