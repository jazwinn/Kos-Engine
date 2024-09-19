#pragma once
#ifndef RIGIDCOM_H
#define RIGIDCOM_H

#include "../Math/vector2.h"
#include "Component.h"


namespace Ecs {


	class RigidBodyComponent : public Component {

	public:

		float Mass{};

	};

}




#endif RIGIDCOM_H