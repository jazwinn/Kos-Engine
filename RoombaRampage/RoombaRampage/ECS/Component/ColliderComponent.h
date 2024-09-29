#ifndef BOXCOM_H
#define BOXCOM_H

#include "../Math/vector2.h"
#include "Component.h"
#include "../Physics/Physics.h"

namespace Ecs {


	class ColliderComponent : public Component {

	public:
		
		vector2::Vec2 Size = {1,1};

		vector2::Vec2 OffSet{};

		unsigned int Layer{};

		bool drawDebug{true};

		physicspipe::EntityType type = physicspipe::EntityType::RECTANGLE;

		float radius{};
		bool isCollided{ false };

	};

}




#endif BOXCOM_H