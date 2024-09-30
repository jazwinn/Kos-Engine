#pragma once
#ifndef RIGIDCOM_H
#define RIGIDCOM_H

#include "../Math/vector2.h"
#include "Component.h"


namespace ecs {


	class RigidBodyComponent : public Component {

	public:

		float m_Mass{};

	};

}




#endif RIGIDCOM_H