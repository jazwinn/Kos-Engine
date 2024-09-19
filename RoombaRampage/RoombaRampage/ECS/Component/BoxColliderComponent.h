#ifndef BOXCOM_H
#define BOXCOM_H

#include "../Math/vector2.h"
#include "Component.h"


namespace Ecs {


	class BoxColliderComponent : public Component {

	public:
		
		Vector2::Vec2 Size{};

		Vector2::Vec2 OffSet{};

		unsigned int Layer{};

	};

}




#endif BOXCOM_H