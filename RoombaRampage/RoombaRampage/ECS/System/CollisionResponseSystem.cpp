#include "../ECS.h"

#include "CollisionResponseSystem.h"
#include "../Physics/Physics.h"

namespace Ecs {

	void CollisionResponseSystem::RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(vecMovementComponentPtr.begin(), vecMovementComponentPtr.end(), [ID](const auto& obj) { return obj->Entity == ID; })
			== vecMovementComponentPtr.end()) {
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

		std::swap(vecRigidBodyComponentPtr[IndexID], vecRigidBodyComponentPtr[IndexLast]);
		std::swap(vecMovementComponentPtr[IndexID], vecMovementComponentPtr[IndexLast]);

		//popback the vector;
		vecRigidBodyComponentPtr.pop_back();
		vecMovementComponentPtr.pop_back();
	}

	void CollisionResponseSystem::Init() {

		// requires both movement component and transform component
		//SystemSignature.set(TypeBoxColliderComponent);
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

		//create physics;
		Physics::classPhysics PysicsPipeline;
		std::vector<Physics::PhysicsData> vecCollisionEntity = PysicsPipeline.PassPhysicsData();

		//for (Physics::p)

		//check for collision
		PysicsPipeline.CollisionCheck(ecs->DeltaTime);

		//clear the entity
		PysicsPipeline.ClearEntites();
	}


}
