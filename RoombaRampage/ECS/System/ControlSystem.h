#ifndef CONTROLSYS_H
#define CONTROLSYS_H

#include "System.h"
#include "../ECS/Component/MovementComponent.h"
#include "../ECS/Component/PlayerComponent.h"
#include "../ECS/ECSList.h"

namespace ecs {

	class ControlSystem : public ISystem {

	public:
		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update() override;

	private:
		//Storage to point to components
		std::vector<PlayerComponent*> m_vecPlayerComponentPtr;
		std::vector<MovementComponent*> m_vecMovementComponentPtr;

	};

}



#endif CONTROLSYS_H
