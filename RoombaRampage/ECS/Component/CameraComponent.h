#ifndef CAMCOM_H
#define CAMCOM_H

#include "Component.h"
#include "../Math/vector2.h"

namespace ecs {


	class CameraComponent : public Component {

	public:
		float m_left;
		float m_right;
		float m_top;
		float m_bottom;
		float m_aspectRatio;

	};

}




#endif CAMCOM_H