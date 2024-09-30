#ifndef COLLSYS_H
#define COLLSYS_H

#include "System.h"
#include "../ECS/ECSList.h"

namespace ecs {

	class CollisionSystem : public ISystem {

	public:
		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update() override;

	private:
		//Storage to point to components
		std::vector<TransformComponent*> m_vecTransformComponentPtr;
		std::vector<ColliderComponent*> m_vecColliderComponentPtr;
		//std::vector<RigidBodyComponent*> vecRigidBodyComponentPtr;
		//std::vector<MovementComponent*> m_vecMovementComponentPtr;

	};

}



#endif COLLSYS_H
