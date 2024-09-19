#ifndef SYSTEM_H
#define SYSTEM_H

#include <bitset>
#include <vector>
#include <algorithm>
#include "../ECS/Component/Component.h"

namespace Ecs {

	enum TypeSystem {
		TypeMovementSystem,




		//render system called the last
		TypeRenderSystem,

		TotalTypeSystem
	};

	class ISystem {

	public:

		compSignature SystemSignature;

		virtual void RegisterSystem(EntityID) = 0;

		virtual void DeregisterSystem(EntityID) = 0;

		virtual void Init() = 0;

		virtual void Update() = 0;


	};

}









#endif SYSTEM_H

