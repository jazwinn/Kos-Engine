/******************************************************************/
/*!
\file      CollisionResponseSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination for the collision
		   respoonse system. It retrieves for collision data from
		   the physics pipeline and reach accordingly.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "../ECS.h"

#include "CollisionResponseSystem.h"
#include "../Physics/Physics.h"
#include "../Graphics/GraphicsPipe.h"

namespace ecs {

	void CollisionResponseSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecRigidBodyComponentPtr.begin(), m_vecRigidBodyComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecRigidBodyComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecColliderComponentPtr.push_back((ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ID));
			m_vecRigidBodyComponentPtr.push_back((RigidBodyComponent*)ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void CollisionResponseSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecRigidBodyComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecRigidBodyComponentPtr.size() - 1;

		std::swap(m_vecColliderComponentPtr[IndexID],  m_vecColliderComponentPtr[IndexLast]);
		std::swap(m_vecRigidBodyComponentPtr[IndexID], m_vecRigidBodyComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);

		//popback the vector;
		m_vecColliderComponentPtr.pop_back();
		m_vecRigidBodyComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
	}

	void CollisionResponseSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPECOLLIDERCOMPONENT);
		m_SystemSignature.set(TYPERIGIDBODYCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);

	}

	void CollisionResponseSystem::m_Update(const std::string& scene) {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecRigidBodyComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}

		//TODO instead of retrieving every loop, just get the address of the vector in the physics pipeline
		physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::getInstance();
		std::vector <std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData>>> vecCollisionEntityPair = PhysicsPipeline->m_RetrievePhysicsDataPair();
		std::unordered_set<ecs::EntityID> ids;
		
		// return if no collision occuring
		if (!vecCollisionEntityPair.size()) return;



		//for (auto& CollidedEntity : vecCollisionEntity) {
		//	// create hash set
		//	ids.insert(CollidedEntity->m_ID);
		//}
		

		for (int n{}; n < m_vecRigidBodyComponentPtr.size(); n++)
		{
			RigidBodyComponent* rigidComp = m_vecRigidBodyComponentPtr[n];
			ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
			ColComp->m_collidedWith.clear();
			//skip component not of the scene
			if (rigidComp->m_scene != scene) continue;	

			const EntityID check_ID = ColComp->m_Entity;
			const auto& iterator = std::find_if(vecCollisionEntityPair.begin(), vecCollisionEntityPair.end(), [check_ID](const auto pair) { return (check_ID == pair.first->m_ID); });
			if (iterator != vecCollisionEntityPair.end()) {
				ColComp->m_collidedWith.push_back(iterator->second->m_ID);
				ColComp->m_isCollided = true;
			}
		}

		
		//for (int i{}; i < vecCollisionEntityPair.size(); ++i) {
		//	get the collider component
		//	ColliderComponent* cc_1 = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]
		//		->m_GetEntityComponent(vecCollisionEntityPair[i].first->m_ID));

		//	store insidet the vector

		//	cc_1->m_collidedWith.push_back(vecCollisionEntityPair[i].second->m_ID);

		//	cc_1->m_isCollided = true;
		//}

	}


}
