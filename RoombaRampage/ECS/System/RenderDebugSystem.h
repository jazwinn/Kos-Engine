#ifndef DEBUGDRAWSYS_H
#define DEBUGDRAWSYS_H

#include "System.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/ColliderComponent.h"
#include "../ECS/Component/RigidBody.h"

#include "../ECS/ECSList.h"

namespace ecs {

	class DebugDrawingSystem : public ISystem {

	public:
		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update() override;

	private:
		//Storage to point to components
		std::vector<TransformComponent*> m_vecTransformComponentPtr;
		std::vector<ColliderComponent*> m_vecColliderComponentPtr;

	};

}



#endif DEBUGDRAWSYS_H
