#ifndef MOVESYS_H
#define MOVESYS_H

#include "System.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/MovementComponent.h"

namespace ecs {

	class MovementSystem : public ISystem {

	public:
		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update() override;

	private:
		//Storage to point to components
		std::vector<TransformComponent*> m_vecTransformComponentPtr;
		std::vector<MovementComponent*> m_vecMovementComponentPtr;

	};

}



#endif MOVESYS_H