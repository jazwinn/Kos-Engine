#include "../ECS.h"

#include "CollisionSystem.h"
#include "../Physics/Physics.h"

namespace Ecs {

	void CollisionSystem::RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(vecTransformComponentPtr.begin(), vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->Entity == ID; })
			== vecTransformComponentPtr.end()) {
			vecTransformComponentPtr.push_back((TransformComponent*)ecs->ECS_CombinedComponentPool[TypeTransformComponent]->GetEntityComponent(ID));
			vecBoxColliderComponentPtr.push_back((BoxColliderComponent*)ecs->ECS_CombinedComponentPool[TypeBoxColliderComponent]->GetEntityComponent(ID));
			vecRigidBodyComponentPtr.push_back((RigidBodyComponent*)ecs->ECS_CombinedComponentPool[TypeRigidBodyComponent]->GetEntityComponent(ID));
			vecMovementComponentPtr.push_back((MovementComponent*)ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->GetEntityComponent(ID));
		}


	}

	void CollisionSystem::DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : vecBoxColliderComponentPtr) {
			if (ComponentPtr->Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = vecBoxColliderComponentPtr.size() - 1;

		std::swap(vecBoxColliderComponentPtr[IndexID], vecBoxColliderComponentPtr[IndexLast]);
		std::swap(vecTransformComponentPtr[IndexID], vecTransformComponentPtr[IndexLast]);
		std::swap(vecRigidBodyComponentPtr[IndexID], vecRigidBodyComponentPtr[IndexLast]);
		std::swap(vecMovementComponentPtr[IndexID], vecMovementComponentPtr[IndexLast]);

		//popback the vector;
		vecBoxColliderComponentPtr.pop_back();
		vecTransformComponentPtr.pop_back();
		vecRigidBodyComponentPtr.pop_back();
		vecMovementComponentPtr.pop_back();
	}

	void CollisionSystem::Init() {

		// requires both movement component and transform component
		SystemSignature.set(TypeBoxColliderComponent);
		SystemSignature.set(TypeRigidBodyComponent);
		SystemSignature.set(TypeMovemmentComponent);
		//SystemSignature.set();

	}

	void CollisionSystem::Update() {

		ECS* ecs = ECS::GetInstance();

		if (vecBoxColliderComponentPtr.size() != vecTransformComponentPtr.size()) {
			std::cout << "Error: Vecotrs container size does not Match" << std::endl;
			return;
		}

		//create physics;
		Physics::classPhysics PysicsPipeline;

		for (int n{}; n < vecTransformComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";

			BoxColliderComponent* BoxComp = vecBoxColliderComponentPtr[n];
			TransformComponent* TransComp = vecTransformComponentPtr[n];
			MovementComponent* MovComp = vecMovementComponentPtr[n];

			
			PysicsPipeline.RetrievePhysicsData(TransComp->scale,TransComp->position, MovComp->Speed * MovComp->Direction, BoxComp->Entity);

			
		}

		//check for collision
		PysicsPipeline.CollisionCheck(ecs->DeltaTime);

		

		std::vector<Physics::PhysicsData> vecCollisionEntity = PysicsPipeline.PassPhysicsData();
		if (vecCollisionEntity.empty()) std::cout << "No collision from Collision System CPP" << std::endl;
		else {
			for (auto& CollidedEntity : vecCollisionEntity) {
				std::cout << "Entity " << CollidedEntity.ID << " is Collided" << std::endl;
			}
		}
		//clear the entity
		PysicsPipeline.ClearEntites();
	}
		

}
