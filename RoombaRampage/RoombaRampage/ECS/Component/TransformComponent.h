#ifndef TRANSCOM_H
#define TRANSCOM_H

#include "Component.h"
#include "../Math/vector2.h"

namespace Ecs {
	class TransformComponent : public Component {

	public:

		Vector2::Vec2 position{0,0};
		float rotation{};
		Vector2::Vec2 scale{1,1};
	};

}



#endif TRANSCOM_H