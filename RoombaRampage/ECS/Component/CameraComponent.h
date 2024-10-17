#ifndef CAMCOM_H
#define CAMCOM_H

#include "Component.h"
#include "../Math/vector2.h"

namespace ecs {


	class CameraComponent : public Component {

	public:
		vector2::Vec2 m_coordinates;
		vector2::Vec2 m_zoom;
		float m_angle;

	};

}




#endif CAMCOM_H