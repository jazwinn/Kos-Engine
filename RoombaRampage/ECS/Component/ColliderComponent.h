#ifndef BOXCOM_H
#define BOXCOM_H

#include "../Math/vector2.h"
#include "Component.h"
#include "../Physics/Physics.h"

namespace ecs {


	class ColliderComponent : public Component {

	public:
		
		vector2::Vec2 m_Size = {1,1};

		vector2::Vec2 m_OffSet{};

		unsigned int m_Layer{};

		bool m_drawDebug{true};

		physicspipe::EntityType m_type = physicspipe::EntityType::RECTANGLE;

		float m_radius{};
		bool m_isCollided{ false };

	};

}




#endif BOXCOM_H