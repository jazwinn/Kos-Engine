#ifndef COLLRESPSYS_H
#define COLLRESPSYS_H

#include "System.h"
#include "../ECS/Component/MovementComponent.h"
#include "../ECS/Component/RigidBody.h"

#include "../ECS/ECSList.h"

namespace ecs {

	class CollisionResponseSystem : public ISystem {

	public:
		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update() override;

	private:
		//Storage to point to components
		//std::vector<TransformComponent*> vecTransformComponentPtr;
		std::vector<ColliderComponent*> m_vecColliderComponentPtr;
		std::vector<RigidBodyComponent*> m_vecRigidBodyComponentPtr;
		std::vector<MovementComponent*> m_vecMovementComponentPtr;

	};

}



#endif COLLRESPSYS_H
