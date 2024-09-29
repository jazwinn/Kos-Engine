#include "../ECS.h"

#include "CollisionSystem.h"
#include "../Physics/Physics.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Debugging/Logging.h"

namespace Ecs {

	void CollisionSystem::RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(vecTransformComponentPtr.begin(), vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->Entity == ID; })
			== vecTransformComponentPtr.end()) {
			vecTransformComponentPtr.push_back((TransformComponent*)ecs->ECS_CombinedComponentPool[TypeTransformComponent]->GetEntityComponent(ID));
			vecColliderComponentPtr.push_back((ColliderComponent*)ecs->ECS_CombinedComponentPool[TypeColliderComponent]->GetEntityComponent(ID));
			//vecRigidBodyComponentPtr.push_back((RigidBodyComponent*)ecs->ECS_CombinedComponentPool[TypeRigidBodyComponent]->GetEntityComponent(ID));
			vecMovementComponentPtr.push_back((MovementComponent*)ecs->ECS_CombinedComponentPool[TypeMovemmentComponent]->GetEntityComponent(ID));
		}
	}

	void CollisionSystem::DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : vecColliderComponentPtr) {
			if (ComponentPtr->Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = vecColliderComponentPtr.size() - 1;

		std::swap(vecColliderComponentPtr[IndexID], vecColliderComponentPtr[IndexLast]);
		std::swap(vecTransformComponentPtr[IndexID], vecTransformComponentPtr[IndexLast]);
		//std::swap(vecRigidBodyComponentPtr[IndexID], vecRigidBodyComponentPtr[IndexLast]);
		std::swap(vecMovementComponentPtr[IndexID], vecMovementComponentPtr[IndexLast]);

		//popback the vector;
		vecColliderComponentPtr.pop_back();
		vecTransformComponentPtr.pop_back();
		//vecRigidBodyComponentPtr.pop_back();
		vecMovementComponentPtr.pop_back();
	}

	void CollisionSystem::Init() {

		// requires both movement component and transform component
		SystemSignature.set(TypeColliderComponent);
		//SystemSignature.set(TypeRigidBodyComponent);
		SystemSignature.set(TypeMovemmentComponent);
		//SystemSignature.set();

	}

	void CollisionSystem::Update() {

		ECS* ecs = ECS::GetInstance();

		if (vecColliderComponentPtr.size() != vecTransformComponentPtr.size()) {
			std::cout << "Error: Vecotrs container size does not Match" << std::endl;
			return;
		}

		//create physics;
		physicspipe::Physics PhysicsPipeline;
		GraphicsPipe* graphicsPipe = GraphicsPipe::funcGetInstance();

		for (int n{}; n < vecTransformComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";

			ColliderComponent* ColComp = vecColliderComponentPtr[n];
			TransformComponent* TransComp = vecTransformComponentPtr[n];
			MovementComponent* MovComp = vecMovementComponentPtr[n];

			if (ColComp->type == physicspipe::EntityType::CIRCLE) {
				PhysicsPipeline.m_SendPhysicsData(ColComp->radius, TransComp->position,TransComp->scale, MovComp->Speed * MovComp->Direction, ColComp->Entity);
			}
			else if (ColComp->type == physicspipe::EntityType::RECTANGLE) {
				PhysicsPipeline.m_SendPhysicsData(ColComp->Size.m_x, ColComp->Size.m_x, TransComp->position,TransComp->scale, MovComp->Speed * MovComp->Direction, ColComp->Entity);
			}
			else {
				LOGGING_ERROR("NO ENTITY TYPE");
			}

			if (ColComp->drawDebug)
			{
				graphicsPipe->debugBoxData.push_back({ 0, glm::vec2{ColComp->Size.m_x * TransComp->scale.m_x, ColComp->Size.m_y * TransComp->scale.m_y}, glm::vec3{TransComp->position.m_x + ColComp->OffSet.m_x,TransComp->position.m_y + ColComp->OffSet.m_y, 0} ,0, 0 });
			}
		}

		//check for collision
		if (vecColliderComponentPtr.size() > 0) {
			PhysicsPipeline.m_CollisionCheck(ecs->DeltaTime);
		}
	
	}
		

}
