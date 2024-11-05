#ifndef CAMCOM_H
#define CAMCOM_H

#include "Component.h"

namespace ecs {


	class CameraComponent : public Component {

	public:
		float m_left;
		float m_right;
		float m_top;
		float m_bottom;
		float m_aspectRatio;

		REFLECTABLE(CameraComponent, m_left, m_right, m_top, m_bottom, m_aspectRatio)
	};

}




#endif CAMCOM_H