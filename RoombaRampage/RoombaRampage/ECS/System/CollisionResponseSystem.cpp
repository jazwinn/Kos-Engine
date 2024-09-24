#include "../ECS.h"

#include "CollisionResponseSystem.h"
#include "../Physics/Physics.h"

namespace Ecs {

	void CollisionResponseSystem::RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(vecMovementComponentPtr.begin(), vecMovementComponentPtr.end(), [ID](const auto& obj) { return obj->Entity == ID; })
			== vecMovementComponentPtr.end()) {
			vecColliderComponentPtr.push_back((ColliderComponent*)ecs->ECS_CombinedComponentPool[TypeColliderComponent]->GetEntityComponent(ID));
			vecRigidBodyComponentPtr.push_back((RigidBodyComponent*)ecs->ECS_CombinedComponentPool[TypeRigidBodyComponent]->GetEntityComponent(ID));
			vecMovementComponentPtr.push_back((MovementComponent*)ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->GetEntityComponent(ID));
		}


	}

	void CollisionResponseSystem::DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : vecMovementComponentPtr) {
			if (ComponentPtr->Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = vecMovementComponentPtr.size() - 1;

		std::swap(vecColliderComponentPtr[IndexID], vecColliderComponentPtr[IndexLast]);
		std::swap(vecRigidBodyComponentPtr[IndexID], vecRigidBodyComponentPtr[IndexLast]);
		std::swap(vecMovementComponentPtr[IndexID], vecMovementComponentPtr[IndexLast]);

		//popback the vector;
		vecColliderComponentPtr.pop_back();
		vecRigidBodyComponentPtr.pop_back();
		vecMovementComponentPtr.pop_back();
	}

	void CollisionResponseSystem::Init() {

		// requires both movement component and transform component
		SystemSignature.set(TypeColliderComponent);
		SystemSignature.set(TypeRigidBodyComponent);
		SystemSignature.set(TypeMovemmentComponent);
		//SystemSignature.set();

	}

	void CollisionResponseSystem::Update() {

		ECS* ecs = ECS::GetInstance();

		if (vecMovementComponentPtr.size() != vecRigidBodyComponentPtr.size()) {
			std::cout << "Error: Vecotrs container size does not Match" << std::endl;
			return;
		}

		Physics::classPhysics PysicsPipeline;
		std::vector<Physics::PhysicsData> vecCollisionEntity = PysicsPipeline.PassPhysicsData();

		if (vecCollisionEntity.empty()); //std::cout << "No collision from Collision System CPP" << std::endl;
		else {
			for (auto& CollidedEntity : vecCollisionEntity) {
				
				std::cout << "Entity " << CollidedEntity.ID << " is Collided" << std::endl;
			
			}
		}
	}


}
