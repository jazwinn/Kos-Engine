#ifndef COLLRESPSYS_H
#define COLLRESPSYS_H

#include "System.h"
#include "../ECS/Component/MovementComponent.h"
#include "../ECS/Component/RigidBody.h"

#include "../ECS/ECSList.h"

namespace Ecs {

	class CollisionResponseSystem : public ISystem {

	public:
		void RegisterSystem(EntityID) override;

		void DeregisterSystem(EntityID) override;

		void Init() override;

		void Update() override;

	private:
		//Storage to point to components
		//std::vector<TransformComponent*> vecTransformComponentPtr;
		std::vector<ColliderComponent*> vecColliderComponentPtr;
		std::vector<RigidBodyComponent*> vecRigidBodyComponentPtr;
		std::vector<MovementComponent*> vecMovementComponentPtr;

	};

}



#endif COLLRESPSYS_H
