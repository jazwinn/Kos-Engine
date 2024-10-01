#include "../ECS.h"

#include "CollisionResponseSystem.h"
#include "../Physics/Physics.h"
#include "../Graphics/GraphicsPipe.h"

namespace ecs {

	void CollisionResponseSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecMovementComponentPtr.begin(), m_vecMovementComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecMovementComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecColliderComponentPtr.push_back((ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ID));
			m_vecRigidBodyComponentPtr.push_back((RigidBodyComponent*)ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(ID));
			m_vecMovementComponentPtr.push_back((MovementComponent*)ecs->m_ECS_CombinedComponentPool[TYPEMOVEMENTCOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void CollisionResponseSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecMovementComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecMovementComponentPtr.size() - 1;

		std::swap(m_vecColliderComponentPtr[IndexID],  m_vecColliderComponentPtr[IndexLast]);
		std::swap(m_vecRigidBodyComponentPtr[IndexID], m_vecRigidBodyComponentPtr[IndexLast]);
		std::swap(m_vecMovementComponentPtr[IndexID],  m_vecMovementComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);

		//popback the vector;
		m_vecColliderComponentPtr.pop_back();
		m_vecRigidBodyComponentPtr.pop_back();
		m_vecMovementComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
	}

	void CollisionResponseSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPECOLLIDERCOMPONENT);
		m_SystemSignature.set(TYPERIGIDBODYCOMPONENT);
		m_SystemSignature.set(TYPEMOVEMENTCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		//SystemSignature.set();

	}

	void CollisionResponseSystem::m_Update() {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecMovementComponentPtr.size() != m_vecRigidBodyComponentPtr.size()) {
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}

		physicspipe::Physics PhysicsPipeline;
		std::vector<std::shared_ptr<physicspipe::PhysicsData>> vecCollisionEntity = PhysicsPipeline.m_RetrievePhysicsData();
		for (int n{}; n < m_vecColliderComponentPtr.size(); n++)
		{
			ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
			ColComp->m_isCollided = false;
		}
		if (vecCollisionEntity.empty()); //std::cout << "No collision from Collision System CPP" << std::endl;
		else {
			//REQUIRES OPTIMIZATION 
			//CollidedEntity is the ID
			for (auto& CollidedEntity : vecCollisionEntity) {
				if (ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_HasComponent(CollidedEntity->m_ID)) {
					MovementComponent* MovCom = (MovementComponent*)ecs->m_ECS_CombinedComponentPool[TYPEMOVEMENTCOMPONENT]->m_GetEntityComponent(CollidedEntity->m_ID);
					ColliderComponent* ColCom = (ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(CollidedEntity->m_ID);
					ColCom->m_isCollided = true;
					MovCom->m_Direction = { 0,0 };
				}	
				
			}


		}
	}


}
