#ifndef TRANSCOM_H
#define TRANSCOM_H

#include "Component.h"
#include "../Math/Vector2.h"

namespace ecs {
	class TransformComponent : public Component {

	public:

		vector2::Vec2 m_position{0,0};
		float m_rotation{};
		vector2::Vec2 m_scale{1,1};
	};

}



#endif TRANSCOM_H