#ifndef BUTTONCOM_H
#define BUTTONCOM_H

#include "Component.h"
#include "../Math/vector2.h"

namespace ecs {


	class ButtonComponent : public Component {

	public:
		vector2::Vec2 m_Position{};
		vector2::Vec2 m_Scale{};
		bool m_IsClick = false;
	};

}




#endif CAMCOM_H