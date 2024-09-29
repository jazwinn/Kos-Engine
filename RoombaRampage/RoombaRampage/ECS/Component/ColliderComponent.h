#ifndef BOXCOM_H
#define BOXCOM_H

#include "../Math/vector2.h"
#include "Component.h"


namespace Ecs {


	class ColliderComponent : public Component {

	public:
		
		Vector2::Vec2 Size = {1,1};

		Vector2::Vec2 OffSet{};

		unsigned int Layer{};

		bool drawDebug{true};

		bool isCollided{ false };

	};

}




#endif BOXCOM_H