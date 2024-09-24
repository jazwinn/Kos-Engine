#ifndef COLLSYS_H
#define COLLSYS_H

#include "System.h"
#include "../ECS/ECSList.h"

namespace Ecs {

	class CollisionSystem : public ISystem {

	public:
		void RegisterSystem(EntityID) override;

		void DeregisterSystem(EntityID) override;

		void Init() override;

		void Update() override;

	private:
		//Storage to point to components
		std::vector<TransformComponent*> vecTransformComponentPtr;
		std::vector<BoxColliderComponent*> vecBoxColliderComponentPtr;
		//std::vector<RigidBodyComponent*> vecRigidBodyComponentPtr;
		std::vector<MovementComponent*> vecMovementComponentPtr;

	};

}



#endif COLLSYS_H
