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

		physicspipe::Physics PhysicsPipeline;
		std::vector<std::shared_ptr<physicspipe::PhysicsData>> vecCollisionEntity = PhysicsPipeline.m_RetrievePhysicsData();
		for (int n{}; n < vecColliderComponentPtr.size(); n++) 
		{
			ColliderComponent* ColComp = vecColliderComponentPtr[n];
			ColComp->isCollided = false;
		}
		if (vecCollisionEntity.empty()); //std::cout << "No collision from Collision System CPP" << std::endl;
		else {
			//REQUIRES OPTIMIZATION 
			//CollidedEntity is the ID
			for (auto& CollidedEntity : vecCollisionEntity) {
				if (ecs->ECS_CombinedComponentPool[TypeRigidBodyComponent]->HasComponent(CollidedEntity->m_ID)) {
					std::cout << "Collided" << std::endl;
					MovementComponent* MovCom = (MovementComponent*)ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->GetEntityComponent(CollidedEntity->m_ID);
					ColliderComponent* ColCom = (ColliderComponent*)ecs->ECS_CombinedComponentPool[TypeColliderComponent]->GetEntityComponent(CollidedEntity->m_ID);
					ColCom->isCollided = true;
					MovCom->Direction = { 0,0 };
				}	
			}

			//if (ecs->ECS_CombinedComponentPool[TypeRigidBodyComponent]->HasComponent(CollidedEntity->ID)) {
			//	MovementComponent* MovCom = (MovementComponent*)ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->GetEntityComponent(CollidedEntity->ID);
			//	MovCom->Direction = { 0,0 };
			//}
		}
	}


}
