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
#include "../Physics/PhysicsCollisionResponse.h"
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

		//ECS* ecs = ECS::m_GetInstance();

		if (m_vecRigidBodyComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}

		//TODO instead of retrieving every loop, just get the address of the vector in the physics pipeline
		physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::m_GetInstance();

		//gets collided
		PhysicsPipeline->m_Update();
		
		std::vector <std::shared_ptr<physicspipe::PhysicsData>> vecCollisionEntity = PhysicsPipeline->m_RetrievePhysicsData();
		std::vector <std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData>>> vecCollisionEntityPair = PhysicsPipeline->m_RetrievePhysicsDataPair();
		std::unordered_set<ecs::EntityID> ids;


		
		
		// return if no collision occuring
		if (!vecCollisionEntityPair.size()) {
			for (int n{}; n < m_vecRigidBodyComponentPtr.size(); n++) {
				ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
				RigidBodyComponent* rigidComp = m_vecRigidBodyComponentPtr[n];
				ColComp->m_collidedWith.clear();
				ColComp->m_triColWith.clear();
				ColComp->m_blockedFlag = -1;
				//skip component not of the scene
				if (rigidComp->m_scene != scene) continue;
				
				std::unordered_set<ecs::EntityID> outOfVecInMap = ColComp->m_keys;
				for (int i = 0; i < ColComp->m_triColWith.size(); i++) {
					if (ColComp->m_keys.find(ColComp->m_triColWith[i]) != ColComp->m_keys.end()) {
						ColComp->m_keys.erase(ColComp->m_triColWith[i]);
					}
				}
				if (!ColComp->m_keys.empty()) {
					const EntityID check_ID = ColComp->m_Entity;
					//for (const auto& elem : ColComp->m_keys) {
					//	if (ColComp->m_collisionState.find(elem) != ColComp->m_collisionState.end()) {
					//		if (ColComp->m_collisionState.find(elem)->second == physicspipe::CollisionState::CONTINUOUS || ColComp->m_collisionState.find(elem)->second == physicspipe::CollisionState::ENTERED) {
					//			const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(elem));
					//			//ColComp->m_collisionState.find(elem)->second = physicspipe::CollisionState::EXIT;
					//			//colComp2->m_collisionState.find(check_ID)->second = physicspipe::CollisionState::EXIT;
					//			//ColComp->m_OnCollisionExit(elem);
					//			//colComp2->m_OnCollisionExit(check_ID);
 				//				ColComp->m_collisionState.erase(elem);
					//			colComp2->m_collisionState.erase(check_ID);
					//			outOfVecInMap.erase(elem);
					//		}
					//	}
					//}
					//ColComp->m_keys = outOfVecInMap;
				}
				
			}
			return;
		}



		//for (auto& CollidedEntity : vecCollisionEntity) {
		//	// create hash set
		//	ids.insert(CollidedEntity->m_ID);
		//}


		

		for (int n{}; n < m_vecRigidBodyComponentPtr.size(); n++)
		{

			RigidBodyComponent* rigidComp = m_vecRigidBodyComponentPtr[n];
			ColliderComponent* ColComp = m_vecColliderComponentPtr[n];

			ColComp->m_collidedWith.clear();
			ColComp->m_triColWith.clear();
			ColComp->m_blockedFlag = -1;
			//skip component not of the scene
			if (rigidComp->m_scene != scene) continue;	

			const EntityID check_ID = ColComp->m_Entity;

			for (int i = 0; i < vecCollisionEntityPair.size(); i++) {
				ecs::EntityID tempID = vecCollisionEntityPair[i].first->m_ID;
				ecs::EntityID tempCollided = vecCollisionEntityPair[i].second->m_ID;
				if (check_ID == tempID) {
					const auto& colComp = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(tempID));
					const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(tempCollided));
					colComp->m_collidedWith.push_back(tempCollided);
					colComp2->m_collidedWith.push_back(tempID);
					colComp->m_triColWith.emplace_back(tempCollided);
					colComp2->m_triColWith.emplace_back(tempID);
					colComp->m_isCollided = true;
					colComp2->m_isCollided = true;
					colComp->m_blockedFlag = vecCollisionEntityPair[i].first->m_collisionFlags;
					colComp2->m_blockedFlag = vecCollisionEntityPair[i].second->m_collisionFlags;
				}
			}

			/*std::unordered_set<ecs::EntityID> outOfVecInMap = ColComp->m_keys;*/
			//const auto& iterator = std::find_if(vecCollisionEntityPair.begin(), vecCollisionEntityPair.end(), [check_ID](const auto pair) { return (static_cast<int>(check_ID) == pair.first->m_ID); });
			for (int i = 0; i < ColComp->m_triColWith.size(); i++) {
				const auto& colComp = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(check_ID));
				ecs::EntityID collidedID = colComp->m_triColWith[i];
				const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(collidedID));
				if (colComp->m_collisionState.find(collidedID) == ColComp->m_collisionState.end()) {
					colComp->m_collisionState.emplace(collidedID, physicspipe::CollisionState::ENTERED);
					colComp2->m_collisionState.emplace(check_ID, physicspipe::CollisionState::ENTERED);
					//colComp->m_OnCollisionEnter(collidedID);
					//colComp2->m_OnCollisionEnter(check_ID);
					colComp->m_keys.emplace(collidedID);
					colComp2->m_keys.emplace(check_ID);
				}
				else if (colComp->m_collisionState.find(collidedID)->second == physicspipe::CollisionState::ENTERED) {
					colComp->m_collisionState.find(collidedID)->second = physicspipe::CollisionState::CONTINUOUS;
					colComp2->m_collisionState.find(check_ID)->second = physicspipe::CollisionState::CONTINUOUS;
					//colComp->m_OnCollisionStay(collidedID);
					//colComp2->m_OnCollisionStay(check_ID);
					colComp->m_keys.emplace(collidedID);
					colComp2->m_keys.emplace(check_ID);
				}
				else if (colComp->m_collisionState.find(collidedID)->second == physicspipe::CollisionState::CONTINUOUS) {
					//colComp->m_OnCollisionStay(collidedID);
					//colComp2->m_OnCollisionStay(check_ID);
				}
			}
			//for (int i = 0; i < ColComp->m_collisionState.size(); i++) {
			//	
			//	if (ColComp->m_collidedWith[i] && ColComp->m_collisionState.find(ColComp->m_collidedWith[i]) != ColComp->m_collisionState.end()) {
			//		continue;
			//	}
			//	else {
			//		const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ColComp->m_collidedWith[i]));
			//		ColComp->m_OnCollisionExit(ColComp->m_collidedWith[i]);
			//		colComp2->m_OnCollisionExit(check_ID);
			//		ColComp->m_collisionState.erase(ColComp->m_collidedWith[i]);
			//		colComp2->m_collisionState.erase(check_ID);
			//		
			//	}
			//}

			

			//if (iterator != vecCollisionEntityPair.end()) {
			//	ColComp->m_collidedWith.push_back(iterator->second->m_ID);
			//	ColComp->m_isCollided = true;
			//	ColComp->m_blockedFlag = iterator->first->m_collisionFlags;

			//	//ECS* ecs = ECS::m_GetInstance();
			//	//const auto& nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(iterator->first->m_ID));
			//	//const auto& tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(iterator->first->m_ID));
			//	//const auto& nc2 = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(iterator->second->m_ID));

			//	//if (iterator->first->type == physicspipe::EntityType::CIRCLE && nc->m_entityTag == "Player" && nc2->m_entityTag == "Wall")
			//	//{
			//	//	/*float xDist = iterator->first->m_position.m_x - iterator->second->m_position.m_x;
			//	//	float yDist = iterator->first->m_position.m_y - iterator->second->m_position.m_y;*/

			//	//	auto SweptCircleAABBCollision = [](float cxStart, float cyStart, float cxEnd, float cyEnd,
			//	//		float radius,
			//	//		float xMin, float yMin, float xMax, float yMax,
			//	//		float& tImpact) {
			//	//			// Expand AABB by radius
			//	//			float exMin = xMin - radius;
			//	//			float exMax = xMax + radius;
			//	//			float eyMin = yMin - radius;
			//	//			float eyMax = yMax + radius;

			//	//			// Calculate direction of movement
			//	//			float dx = cxEnd - cxStart;
			//	//			float dy = cyEnd - cyStart;

			//	//			// Time intervals for each axis
			//	//			float tMin = 0.0f, tMax = 1.0f;

			//	//			// Check each axis
			//	//			if (dx != 0.0f) {
			//	//				float t1 = (exMin - cxStart) / dx;
			//	//				float t2 = (exMax - cxStart) / dx;
			//	//				if (t1 > t2) std::swap(t1, t2);
			//	//				tMin = std::max(tMin, t1);
			//	//				tMax = std::min(tMax, t2);
			//	//			}

			//	//			if (dy != 0.0f) {
			//	//				float t1 = (eyMin - cyStart) / dy;
			//	//				float t2 = (eyMax - cyStart) / dy;
			//	//				if (t1 > t2) std::swap(t1, t2);
			//	//				tMin = std::max(tMin, t1);
			//	//				tMax = std::min(tMax, t2);
			//	//			}

			//	//			// Check if there is a valid intersection
			//	//			if (tMin <= tMax) {
			//	//				tImpact = tMin; // Time of impact
			//	//				return true;
			//	//			}

			//	//			return false; // No collision
			//	//		};

			//	//	

			//	//	float xMin = iterator->second->m_position.m_x - iterator->second->m_scale.m_x / 2.f;
			//	//	float xMax = iterator->second->m_position.m_x + iterator->second->m_scale.m_x / 2.f;

			//	//	float yMin = iterator->second->m_position.m_y - iterator->second->m_scale.m_y / 2.f;
			//	//	float yMax = iterator->second->m_position.m_y + iterator->second->m_scale.m_y / 2.f;

			//	//	float radius = iterator->first->m_scale.m_x/2.f;
			//	//	
			//	//	float nearestX = std::max(xMin, std::min(iterator->first->m_position.m_x, xMax));
			//	//	float nearestY = std::max(yMin, std::min(iterator->first->m_position.m_y, yMax));

			//	//	float distanceFromEdge = std::sqrt((iterator->first->m_position.m_x - nearestX) * (iterator->first->m_position.m_x - nearestX) + (iterator->first->m_position.m_y - nearestY) * (iterator->first->m_position.m_y - nearestY));

			//	//	std::cout << "Old" << iterator->first->m_position.m_x << std::endl;

			//	//	std::cout << "New" << tc->m_position.m_x << std::endl;

			//	//	float t;
			//	//	//if (distanceFromEdge < radius) {
			//	//	//	float penetration = radius - distanceFromEdge;

			//	//	//	// Normalize the resolution direction
			//	//	//	float resolutionDirX = (iterator->first->m_position.m_x - nearestX) / distanceFromEdge;
			//	//	//	float resolutionDirY = (iterator->first->m_position.m_y - nearestY) / distanceFromEdge;

			//	//	//	std::cout << penetration << std::endl; std::cout << penetration << std::endl;

			//	//	//	// Adjust circle position
			//	//	//	iterator->first->m_position.m_x += penetration * resolutionDirX;
			//	//	//	iterator->first->m_position.m_y += penetration * resolutionDirY;
			//	//	//	tc->m_position = iterator->first->m_position;
			//	//	//}

			//	//	if (SweptCircleAABBCollision(iterator->first->m_position.m_x, iterator->first->m_position.m_y, tc->m_position.m_x, tc->m_position.m_y
			//	//		, radius, xMin, yMin, xMax, yMax, t))
			//	//	{
			//	//		std::cout << "T value" << t << std::endl;
			//	//		tc->m_position.m_x = iterator->first->m_position.m_x + t * (tc->m_position.m_x - iterator->first->m_position.m_x);
			//	//		tc->m_position.m_y = iterator->first->m_position.m_y + t * (tc->m_position.m_y - iterator->first->m_position.m_y);
			//	//	}
			//	//	
			//	//}

			//	//std::cout << "Entity 1 " << iterator->first->m_collisionFlags << std::endl;
			//	//std::cout << "Entity 2 " << iterator->second->m_collisionFlags << std::endl;
			//	// 
			//	// 
			//	/*std::cout << "Entity  " << iterator->first->m_ID << " " << iterator->first->m_collisionFlags << std::endl;
			//	std::cout << "Entity  " << iterator->second->m_ID << " " << iterator->second->m_collisionFlags << std::endl;*/
			//	//std::cout << iterator->first->m_position.m_x << " " << iterator->first->m_position.m_y << std::endl;
			//	//for (size_t i = 0; i < iterator->first->m_blockedDirections.size(); ++i) {
			//	//	std::cout << iterator->first->m_blockedDirections[i] << " ";
			//	//}
			//	//std::cout << std::endl;
			//	//std::cout << iterator->second->m_position.m_x << " " << iterator->second->m_position.m_y << std::endl;
			//	//for (size_t i = 0; i < iterator->second->m_blockedDirections.size(); ++i) {
			//	//	std::cout << iterator->second->m_blockedDirections[i] << " ";
			//	//}
			//	//std::cout << std::endl;
			//}
			
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
