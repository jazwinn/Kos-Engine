#ifndef BUTTONCOM_H
#define BUTTONCOM_H

#include "Component.h"
#include "../Math/vector2.h"

namespace ecs {


	class ButtonComponent : public Component {

	public:
		vector2::Vec2 m_ButtonPosition{};
		vector2::Vec2 m_Scale{};
	};

}




#endif CAMCOM_H