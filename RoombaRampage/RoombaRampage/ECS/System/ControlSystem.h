#ifndef CONTROLSYS_H
#define CONTROLSYS_H

#include "System.h"
#include "../ECS/Component/MovementComponent.h"
#include "../ECS/Component/PlayerComponent.h"
#include "../ECS/ECSList.h"

namespace Ecs {

	class ControlSystem : public ISystem {

	public:
		void RegisterSystem(EntityID) override;

		void DeregisterSystem(EntityID) override;

		void Init() override;

		void Update() override;

	private:
		//Storage to point to components
		std::vector<PlayerComponent*> vecPlayerComponentPtr;
		std::vector<MovementComponent*> vecMovementComponentPtr;

	};

}



#endif CONTROLSYS_H
