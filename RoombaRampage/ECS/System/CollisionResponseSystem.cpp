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
#include "../ECS/Hierachy.h"
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
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
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
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecColliderComponentPtr.pop_back();
		m_vecRigidBodyComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
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
		physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::m_GetInstance();

		//gets collided
		PhysicsPipeline->m_Update();
		
		std::vector <std::shared_ptr<physicspipe::PhysicsData>> vecCollisionEntity = PhysicsPipeline->m_RetrievePhysicsData();
		std::vector <std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData>>> vecCollisionEntityPair = PhysicsPipeline->m_RetrievePhysicsDataPair();
		std::vector < std::pair < std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData>>, std::pair<vector2::Vec2, float>>> vecCollisionEntityPairWithVector = PhysicsPipeline->m_RetrievePhysicsDataPairWithVector();
		std::unordered_set<ecs::EntityID> ids;


	#pragma region commented
		
		//// return if no collision occuring
		//if (!vecCollisionEntityPair.size()) {
		//	for (int n{}; n < m_vecRigidBodyComponentPtr.size(); n++) {
		//		ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
		//		RigidBodyComponent* rigidComp = m_vecRigidBodyComponentPtr[n];
		//		NameComponent* NameComp = m_vecNameComponentPtr[n];
		//		ColComp->m_collidedWith.clear();
		//		ColComp->m_triColWith.clear();
		//		ColComp->m_blockedFlag = 0;
		//		ColComp->m_bits.reset();
		//		ColComp->m_contactPoints.clear();
		//		ColComp->m_bits.reset();
		//		//skip component not of the scene
		//		if ((ColComp->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer)) continue;
		//		
		//		std::unordered_set<ecs::EntityID> outOfVecInMap = ColComp->m_keys;
		//		for (int i = 0; i < ColComp->m_triColWith.size(); i++) {
		//			if (ColComp->m_keys.find(ColComp->m_triColWith[i]) != ColComp->m_keys.end()) {
		//				ColComp->m_keys.erase(ColComp->m_triColWith[i]);
		//			}
		//		}
		//		if (!ColComp->m_keys.empty()) {
		//			//const EntityID check_ID = ColComp->m_Entity;
		//			//for (const auto& elem : ColComp->m_keys) {
		//			//	if (ColComp->m_collisionState.find(elem) != ColComp->m_collisionState.end()) {
		//			//		if (ColComp->m_collisionState.find(elem)->second == physicspipe::CollisionState::CONTINUOUS || ColComp->m_collisionState.find(elem)->second == physicspipe::CollisionState::ENTERED) {
		//			//			const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(elem));
		//			//			//ColComp->m_collisionState.find(elem)->second = physicspipe::CollisionState::EXIT;
		//			//			//colComp2->m_collisionState.find(check_ID)->second = physicspipe::CollisionState::EXIT;
		//			//			//ColComp->m_OnCollisionExit(elem);
		//			//			//colComp2->m_OnCollisionExit(check_ID);
 	//			//				ColComp->m_collisionState.erase(elem);
		//			//			colComp2->m_collisionState.erase(check_ID);
		//			//			outOfVecInMap.erase(elem);
		//			//		}
		//			//	}
		//			//}
		//			//ColComp->m_keys = outOfVecInMap;
		//		}
		//		
		//	}
		//	return;
		//}



		////for (auto& CollidedEntity : vecCollisionEntity) {
		////	// create hash set
		////	ids.insert(CollidedEntity->m_ID);
		////}


		//

		//for (int n{}; n < m_vecRigidBodyComponentPtr.size(); n++)
		//{

		//	RigidBodyComponent* rigidComp = m_vecRigidBodyComponentPtr[n];
		//	ColliderComponent* ColComp = m_vecColliderComponentPtr[n];

		//	ColComp->m_collidedWith.clear();
		//	ColComp->m_triColWith.clear();
		//	ColComp->m_blockedFlag = 0;
		//	ColComp->m_bits.reset();
		//	//skip component not of the scene
		//	if (rigidComp->m_scene != scene) continue;	

		//	const EntityID check_ID = ColComp->m_Entity;

		//	for (int i = 0; i < vecCollisionEntityPair.size(); i++) {
		//		ecs::EntityID tempID = vecCollisionEntityPair[i].first->m_ID;
		//		ecs::EntityID tempCollided = vecCollisionEntityPair[i].second->m_ID;
		//		if (check_ID == tempID) {
		//			const auto& colComp = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(tempID));
		//			const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(tempCollided));
		//			colComp2->m_collidedWith.clear();
		//			colComp2->m_triColWith.clear();
		//			colComp2->m_blockedFlag = 0;
		//			colComp->m_collidedWith.push_back(tempCollided);
		//			colComp2->m_collidedWith.push_back(tempID);
		//			colComp->m_triColWith.emplace_back(tempCollided);
		//			colComp2->m_triColWith.emplace_back(tempID);
		//			colComp->m_isCollided = true;
		//			colComp2->m_isCollided = true;
		//			/*colComp->m_blockedFlag = vecCollisionEntityPair[i].first->m_collisionFlags;
		//			colComp2->m_blockedFlag = vecCollisionEntityPair[i].second->m_collisionFlags;*/
		//		}
		//	}

		//	/*std::unordered_set<ecs::EntityID> outOfVecInMap = ColComp->m_keys;*/
		//	//const auto& iterator = std::find_if(vecCollisionEntityPair.begin(), vecCollisionEntityPair.end(), [check_ID](const auto pair) { return (static_cast<int>(check_ID) == pair.first->m_ID); });
		//	for (int i = 0; i < ColComp->m_triColWith.size(); i++) {
		//		const auto& colComp = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(check_ID));
		//		ecs::EntityID collidedID = colComp->m_triColWith[i];
		//		const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(collidedID));
		//		if (colComp->m_collisionState.find(collidedID) == ColComp->m_collisionState.end()) {
		//			colComp->m_collisionState.emplace(collidedID, physicspipe::CollisionState::ENTERED);
		//			colComp2->m_collisionState.emplace(check_ID, physicspipe::CollisionState::ENTERED);
		//			//colComp->m_OnCollisionEnter(collidedID);
		//			//colComp2->m_OnCollisionEnter(check_ID);
		//			colComp->m_keys.emplace(collidedID);
		//			colComp2->m_keys.emplace(check_ID);
		//		}
		//		else if (colComp->m_collisionState.find(collidedID)->second == physicspipe::CollisionState::ENTERED) {
		//			colComp->m_collisionState.find(collidedID)->second = physicspipe::CollisionState::CONTINUOUS;
		//			colComp2->m_collisionState.find(check_ID)->second = physicspipe::CollisionState::CONTINUOUS;
		//			//colComp->m_OnCollisionStay(collidedID);
		//			//colComp2->m_OnCollisionStay(check_ID);
		//			colComp->m_keys.emplace(collidedID);
		//			colComp2->m_keys.emplace(check_ID);
		//		}
		//		else if (colComp->m_collisionState.find(collidedID)->second == physicspipe::CollisionState::CONTINUOUS) {
		//			//colComp->m_OnCollisionStay(collidedID);
		//			//colComp2->m_OnCollisionStay(check_ID);
		//		}
		//	}
		//	//for (int i = 0; i < ColComp->m_collisionState.size(); i++) {
		//	//	
		//	//	if (ColComp->m_collidedWith[i] && ColComp->m_collisionState.find(ColComp->m_collidedWith[i]) != ColComp->m_collisionState.end()) {
		//	//		continue;
		//	//	}
		//	//	else {
		//	//		const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ColComp->m_collidedWith[i]));
		//	//		ColComp->m_OnCollisionExit(ColComp->m_collidedWith[i]);
		//	//		colComp2->m_OnCollisionExit(check_ID);
		//	//		ColComp->m_collisionState.erase(ColComp->m_collidedWith[i]);
		//	//		colComp2->m_collisionState.erase(check_ID);
		//	//		
		//	//	}
		//	//}

		//	

		//	//if (iterator != vecCollisionEntityPair.end()) {
		//	//	ColComp->m_collidedWith.push_back(iterator->second->m_ID);
		//	//	ColComp->m_isCollided = true;
		//	//	ColComp->m_blockedFlag = iterator->first->m_collisionFlags;

		//	//	//ECS* ecs = ECS::m_GetInstance();
		//	//	//const auto& nc = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(iterator->first->m_ID));
		//	//	//const auto& tc = static_cast<ecs::TransformComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(iterator->first->m_ID));
		//	//	//const auto& nc2 = static_cast<ecs::NameComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPENAMECOMPONENT]->m_GetEntityComponent(iterator->second->m_ID));

		//	//	//if (iterator->first->type == physicspipe::EntityType::CIRCLE && nc->m_entityTag == "Player" && nc2->m_entityTag == "Wall")
		//	//	//{
		//	//	//	/*float xDist = iterator->first->m_position.m_x - iterator->second->m_position.m_x;
		//	//	//	float yDist = iterator->first->m_position.m_y - iterator->second->m_position.m_y;*/

		//	//	//	auto SweptCircleAABBCollision = [](float cxStart, float cyStart, float cxEnd, float cyEnd,
		//	//	//		float radius,
		//	//	//		float xMin, float yMin, float xMax, float yMax,
		//	//	//		float& tImpact) {
		//	//	//			// Expand AABB by radius
		//	//	//			float exMin = xMin - radius;
		//	//	//			float exMax = xMax + radius;
		//	//	//			float eyMin = yMin - radius;
		//	//	//			float eyMax = yMax + radius;

		//	//	//			// Calculate direction of movement
		//	//	//			float dx = cxEnd - cxStart;
		//	//	//			float dy = cyEnd - cyStart;

		//	//	//			// Time intervals for each axis
		//	//	//			float tMin = 0.0f, tMax = 1.0f;

		//	//	//			// Check each axis
		//	//	//			if (dx != 0.0f) {
		//	//	//				float t1 = (exMin - cxStart) / dx;
		//	//	//				float t2 = (exMax - cxStart) / dx;
		//	//	//				if (t1 > t2) std::swap(t1, t2);
		//	//	//				tMin = std::max(tMin, t1);
		//	//	//				tMax = std::min(tMax, t2);
		//	//	//			}

		//	//	//			if (dy != 0.0f) {
		//	//	//				float t1 = (eyMin - cyStart) / dy;
		//	//	//				float t2 = (eyMax - cyStart) / dy;
		//	//	//				if (t1 > t2) std::swap(t1, t2);
		//	//	//				tMin = std::max(tMin, t1);
		//	//	//				tMax = std::min(tMax, t2);
		//	//	//			}

		//	//	//			// Check if there is a valid intersection
		//	//	//			if (tMin <= tMax) {
		//	//	//				tImpact = tMin; // Time of impact
		//	//	//				return true;
		//	//	//			}

		//	//	//			return false; // No collision
		//	//	//		};

		//	//	//	

		//	//	//	float xMin = iterator->second->m_position.m_x - iterator->second->m_scale.m_x / 2.f;
		//	//	//	float xMax = iterator->second->m_position.m_x + iterator->second->m_scale.m_x / 2.f;

		//	//	//	float yMin = iterator->second->m_position.m_y - iterator->second->m_scale.m_y / 2.f;
		//	//	//	float yMax = iterator->second->m_position.m_y + iterator->second->m_scale.m_y / 2.f;

		//	//	//	float radius = iterator->first->m_scale.m_x/2.f;
		//	//	//	
		//	//	//	float nearestX = std::max(xMin, std::min(iterator->first->m_position.m_x, xMax));
		//	//	//	float nearestY = std::max(yMin, std::min(iterator->first->m_position.m_y, yMax));

		//	//	//	float distanceFromEdge = std::sqrt((iterator->first->m_position.m_x - nearestX) * (iterator->first->m_position.m_x - nearestX) + (iterator->first->m_position.m_y - nearestY) * (iterator->first->m_position.m_y - nearestY));

		//	//	//	std::cout << "Old" << iterator->first->m_position.m_x << std::endl;

		//	//	//	std::cout << "New" << tc->m_position.m_x << std::endl;

		//	//	//	float t;
		//	//	//	//if (distanceFromEdge < radius) {
		//	//	//	//	float penetration = radius - distanceFromEdge;

		//	//	//	//	// Normalize the resolution direction
		//	//	//	//	float resolutionDirX = (iterator->first->m_position.m_x - nearestX) / distanceFromEdge;
		//	//	//	//	float resolutionDirY = (iterator->first->m_position.m_y - nearestY) / distanceFromEdge;

		//	//	//	//	std::cout << penetration << std::endl; std::cout << penetration << std::endl;

		//	//	//	//	// Adjust circle position
		//	//	//	//	iterator->first->m_position.m_x += penetration * resolutionDirX;
		//	//	//	//	iterator->first->m_position.m_y += penetration * resolutionDirY;
		//	//	//	//	tc->m_position = iterator->first->m_position;
		//	//	//	//}

		//	//	//	if (SweptCircleAABBCollision(iterator->first->m_position.m_x, iterator->first->m_position.m_y, tc->m_position.m_x, tc->m_position.m_y
		//	//	//		, radius, xMin, yMin, xMax, yMax, t))
		//	//	//	{
		//	//	//		std::cout << "T value" << t << std::endl;
		//	//	//		tc->m_position.m_x = iterator->first->m_position.m_x + t * (tc->m_position.m_x - iterator->first->m_position.m_x);
		//	//	//		tc->m_position.m_y = iterator->first->m_position.m_y + t * (tc->m_position.m_y - iterator->first->m_position.m_y);
		//	//	//	}
		//	//	//	
		//	//	//}

		//	//	//std::cout << "Entity 1 " << iterator->first->m_collisionFlags << std::endl;
		//	//	//std::cout << "Entity 2 " << iterator->second->m_collisionFlags << std::endl;
		//	//	// 
		//	//	// 
		//	//	/*std::cout << "Entity  " << iterator->first->m_ID << " " << iterator->first->m_collisionFlags << std::endl;
		//	//	std::cout << "Entity  " << iterator->second->m_ID << " " << iterator->second->m_collisionFlags << std::endl;*/
		//	//	//std::cout << iterator->first->m_position.m_x << " " << iterator->first->m_position.m_y << std::endl;
		//	//	//for (size_t i = 0; i < iterator->first->m_blockedDirections.size(); ++i) {
		//	//	//	std::cout << iterator->first->m_blockedDirections[i] << " ";
		//	//	//}
		//	//	//std::cout << std::endl;
		//	//	//std::cout << iterator->second->m_position.m_x << " " << iterator->second->m_position.m_y << std::endl;
		//	//	//for (size_t i = 0; i < iterator->second->m_blockedDirections.size(); ++i) {
		//	//	//	std::cout << iterator->second->m_blockedDirections[i] << " ";
		//	//	//}
		//	//	//std::cout << std::endl;
		//	//}
		//	
		//}

		//
		////for (int i{}; i < vecCollisionEntityPair.size(); ++i) {
		////	get the collider component
		////	ColliderComponent* cc_1 = static_cast<ecs::ColliderComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]
		////		->m_GetEntityComponent(vecCollisionEntityPair[i].first->m_ID));

		////	store insidet the vector

		////	cc_1->m_collidedWith.push_back(vecCollisionEntityPair[i].second->m_ID);

		////	cc_1->m_isCollided = true;
		////}
		//physicspipe::m_FindContactPoints();
		//physicspipe::m_FindCollisionFlags();

	#pragma endregion
	//Resolving only Wall with (player/enemy)
		if (!vecCollisionEntityPairWithVector.empty()) {
			for (int i{}; i < vecCollisionEntityPairWithVector.size(); ++i) {
				std::pair < std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData>>, std::pair<vector2::Vec2, float>> val = vecCollisionEntityPairWithVector[i];
				EntityID obj1_EntityID = val.first.first.get()->m_ID;
				EntityID obj2_EntityID = val.first.second.get()->m_ID;

				NameComponent* obj1_NC = (NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(obj1_EntityID);
				NameComponent* obj2_NC = (NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(obj2_EntityID);
				TransformComponent* obj1_TC = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(obj1_EntityID);
				TransformComponent* obj2_TC = (TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(obj2_EntityID);
				ColliderComponent* obj1_CC = (ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(obj1_EntityID);
				ColliderComponent* obj2_CC = (ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(obj2_EntityID);
				RigidBodyComponent* obj1_RC = (RigidBodyComponent*)ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(obj1_EntityID);
				RigidBodyComponent* obj2_RC = (RigidBodyComponent*)ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(obj2_EntityID);

				if (obj2_NC->m_entityTag != "Wall") {
					obj2_CC->m_isCollided = 1.0f;
				}
				if (obj1_NC->m_entityTag != "Wall") {
					obj1_CC->m_isCollided = 1.0f;
				}

				if (obj1_CC->m_CollisionCheck == false && obj2_CC->m_CollisionCheck == false)
					continue;

				if (obj1_CC->m_CollisionCheck && obj2_CC->m_CollisionCheck && obj1_RC != NULL && obj2_RC != NULL) {
					obj1_TC->m_position += (-val.second.first * val.second.second);
					obj2_TC->m_position += (val.second.first * val.second.second);
					obj1_RC->m_Velocity = {};
					obj2_RC->m_Velocity = {};
					obj2_CC->m_isCollided = 1.0f;
					obj1_CC->m_isCollided = 1.0f;
				}


				if (obj1_NC->m_entityTag == "Wall") {
					if (obj2_NC->m_entityTag != "Wall") {
						if (std::find_if(m_vecRigidBodyComponentPtr.begin(), m_vecRigidBodyComponentPtr.end(), [obj2_EntityID](const auto& obj) { return obj->m_Entity == obj2_EntityID; })
							!= m_vecRigidBodyComponentPtr.end()) {
							vector2::Vec2 toMove = val.second.first * val.second.second; //DISTANCE TO SHIFT BACK
							obj2_TC->m_position += toMove;
							//obj2_RC->m_Velocity = -obj2_RC->m_Velocity;
						}
					}
				}
				if (obj2_NC->m_entityTag == "Wall") {
					if (obj1_NC->m_entityTag != "Wall") {
						if (std::find_if(m_vecRigidBodyComponentPtr.begin(), m_vecRigidBodyComponentPtr.end(), [obj1_EntityID](const auto& obj) { return obj->m_Entity == obj1_EntityID; })
							!= m_vecRigidBodyComponentPtr.end()) {
							vector2::Vec2 toMove = -val.second.first * val.second.second;
							obj1_TC->m_position += toMove;
							//obj1_RC->m_Velocity = -obj1_RC->m_Velocity;
						}
					}
				}
			}
		}

		////UPDATE THE TRANSFORM MATRIX
		//for (int n{}; n < m_vecTransformComponentPtr.size(); n++)
		//{
		//	TransformComponent* transformComp = m_vecTransformComponentPtr[n];
		//	NameComponent* NameComp = m_vecNameComponentPtr[n];

		//	//skip component not of the scene
		//	if ((transformComp->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer)) continue;

		//	transformComp->m_transformation = mat3x3::Mat3Transform(transformComp->m_position, transformComp->m_scale, transformComp->m_rotation);
		//	vector2::Vec2 pos{}, scale_s{};
		//	float rot{};
		//	mat3x3::Mat3Decompose(transformComp->m_transformation, pos, scale_s, rot);
		//	//std::cout << rot << std::endl;
		//	//std::cout << scale_s.m_x << " " << scale_s.m_y << std::endl;
		//	if (!transformComp->m_haveParent) {
		//		continue;
		//	}
		//	//get parents coordinate
		//	if (!ecs::Hierachy::m_GetParent(transformComp->m_Entity).has_value()) {
		//		// no parnet
		//		return;
		//	}

		//	EntityID parentID = ecs::Hierachy::m_GetParent(transformComp->m_Entity).value();
		//	TransformComponent* parentComp{ nullptr };
		//	for (auto& com : m_vecTransformComponentPtr) {
		//		if (com->m_Entity == parentID) {
		//			parentComp = com;
		//		}
		//	}
		//	if (!parentComp) continue;


		//	mat3x3::Mat3x3 parentTransformation = parentComp->m_transformation;

		//	vector2::Vec2 translate;
		//	vector2::Vec2 scale;
		//	float rotate;
		//	mat3x3::Mat3Decompose(parentTransformation, translate, scale, rotate);
		//	mat3x3::Mat3x3 translateMatrix;
		//	mat3x3::Mat3x3 translateBackMatrix;
		//	mat3x3::Mat3x3 translateToOriginMatrix;
		//	mat3x3::Mat3x3 scaleMatrix;
		//	mat3x3::Mat3x3 rotateMatrix;

		//	transformComp->m_localChildTransformation = mat3x3::Mat3Transform(transformComp->m_position, vector2::Vec2{ transformComp->m_scale.m_x, transformComp->m_scale.m_y }, transformComp->m_rotation);

		//	transformComp->m_transformation = mat3x3::Mat3Transform(translate, vector2::Vec2{ transformComp->m_scale.m_x, transformComp->m_scale.m_y }, 0);

		//	//Set Child Position to Follow Parent
		//	transformComp->m_transformation.m_e20 += transformComp->m_position.m_x;
		//	transformComp->m_transformation.m_e21 += transformComp->m_position.m_y;


		//	mat3x3::Mat3Scale(scaleMatrix, scale.m_x, scale.m_y);
		//	mat3x3::Mat3RotDeg(rotateMatrix, rotate);
		//	mat3x3::Mat3Translate(translateToOriginMatrix, -translate.m_x, -translate.m_y);
		//	mat3x3::Mat3Translate(translateBackMatrix, translate.m_x, (translate.m_y));
		//	transformComp->m_transformation = translateBackMatrix * rotateMatrix * scaleMatrix * translateToOriginMatrix * transformComp->m_transformation;

		//	mat3x3::Mat3RotDeg(rotateMatrix, transformComp->m_rotation);
		//	mat3x3::Mat3Scale(scaleMatrix, transformComp->m_scale.m_x, transformComp->m_scale.m_y);
		//	mat3x3::Mat3Translate(translateToOriginMatrix, -transformComp->m_transformation.m_e20, -transformComp->m_transformation.m_e21);
		//	mat3x3::Mat3Translate(translateBackMatrix, transformComp->m_transformation.m_e20, transformComp->m_transformation.m_e21);

		//	transformComp->m_transformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * transformComp->m_transformation;


		//	//transformComp->m_transformation = parentTransformation * transformComp->m_transformation;
		//}

		////uppdate the rendering
		//for (int n{}; n < m_vecTransformComponentPtr.size(); n++) {
		//	//std::cout << "Entity: " << n << "Movement System is getting Updated";

		//	ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
		//	TransformComponent* TransComp = m_vecTransformComponentPtr[n];
		//	NameComponent* NameComp = m_vecNameComponentPtr[n];
		//	EntityID id = ColComp->m_Entity;

		//	//skip component not of the scene
		//	if ((ColComp->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer)) continue;



		//	//if movement component is present, do dynamic collision
		//	vector2::Vec2 velocity{};
		//	vector2::Vec2 prevPos{};
		//	if (ecs->m_ECS_EntityMap[id].test(TYPERIGIDBODYCOMPONENT)) {

		//		RigidBodyComponent* rigidComp = static_cast<RigidBodyComponent*>(ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(id));

		//		velocity = rigidComp->m_Velocity;
		//		prevPos = rigidComp->m_PrevPos;

		//	}

		//	vector2::Vec2 position{ TransComp->m_transformation.m_e20,TransComp->m_transformation.m_e21 };

		//	mat3x3::Mat3x3 translateMatrix;
		//	mat3x3::Mat3x3 translateBackMatrix;
		//	mat3x3::Mat3x3 translateToOriginMatrix;
		//	mat3x3::Mat3x3 scaleMatrix;
		//	mat3x3::Mat3x3 rotateMatrix;
		//	vector2::Vec2 pos{}, scale{};
		//	float rot{};

		//	//If Entity Has A Parent
		//	if (TransComp->m_haveParent) {
		//		EntityID parentID = ecs::Hierachy::m_GetParent(TransComp->m_Entity).value();
		//		TransformComponent* parentComp{ nullptr };
		//		for (auto& com : m_vecTransformComponentPtr) {
		//			if (com->m_Entity == parentID) {
		//				parentComp = com;
		//			}
		//		}
		//		if (!parentComp) continue;

		//		if (ColComp->m_type == physicspipe::EntityType::CIRCLE) {
		//			mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(TransComp->m_position, vector2::Vec2{ 1.f, 1.f }, 0);

		//			debugTransformation.m_e20 += parentComp->m_position.m_x;
		//			debugTransformation.m_e21 += parentComp->m_position.m_y;

		//			mat3x3::Mat3Scale(scaleMatrix, parentComp->m_scale.m_x, parentComp->m_scale.m_y);
		//			mat3x3::Mat3RotDeg(rotateMatrix, parentComp->m_rotation);
		//			mat3x3::Mat3Translate(translateToOriginMatrix, -parentComp->m_position.m_x, -parentComp->m_position.m_y);
		//			mat3x3::Mat3Translate(translateBackMatrix, parentComp->m_position.m_x, (parentComp->m_position.m_y));
		//			debugTransformation = translateBackMatrix * rotateMatrix * scaleMatrix * translateToOriginMatrix * debugTransformation;


		//			mat3x3::Mat3RotDeg(rotateMatrix, TransComp->m_rotation);
		//			mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
		//			mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

		//			debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;


		//			debugTransformation = debugTransformation * mat3x3::Mat3Transform(ColComp->m_OffSet, vector2::Vec2{ ColComp->m_radius * 2.f / parentComp->m_scale.m_x, ColComp->m_radius * 2.f / parentComp->m_scale.m_y }, 0);
		//			ColComp->m_collider_Transformation = debugTransformation;

		//			mat3x3::Mat3Decompose(ColComp->m_collider_Transformation, pos, scale, rot);
		//		}
		//		else if (ColComp->m_type == physicspipe::EntityType::RECTANGLE) {
		//			mat3x3::Mat3x3 child_Transform = mat3x3::Mat3Transform(TransComp->m_position, TransComp->m_scale, 0);

		//			child_Transform.m_e20 += parentComp->m_position.m_x;
		//			child_Transform.m_e21 += parentComp->m_position.m_y;

		//			mat3x3::Mat3Scale(scaleMatrix, parentComp->m_scale.m_x, parentComp->m_scale.m_y);
		//			mat3x3::Mat3RotDeg(rotateMatrix, parentComp->m_rotation);
		//			mat3x3::Mat3Translate(translateToOriginMatrix, -parentComp->m_position.m_x, -parentComp->m_position.m_y);
		//			mat3x3::Mat3Translate(translateBackMatrix, parentComp->m_position.m_x, (parentComp->m_position.m_y));
		//			child_Transform = translateBackMatrix * rotateMatrix * scaleMatrix * translateToOriginMatrix * child_Transform;

		//			mat3x3::Mat3RotDeg(rotateMatrix, TransComp->m_rotation);
		//			mat3x3::Mat3Translate(translateToOriginMatrix, -child_Transform.m_e20, -child_Transform.m_e21);
		//			mat3x3::Mat3Translate(translateBackMatrix, child_Transform.m_e20, child_Transform.m_e21);

		//			child_Transform = translateBackMatrix * rotateMatrix * translateToOriginMatrix * child_Transform;

		//			mat3x3::Mat3x3 final_Transform = child_Transform * mat3x3::Mat3Transform(ColComp->m_OffSet, vector2::Vec2{ ColComp->m_Size.m_x , ColComp->m_Size.m_y }, 0);

		//			mat3x3::Mat3x3 debugTransformation = final_Transform;


		//			ColComp->m_collider_Transformation = debugTransformation;

		//		}
		//		else {
		//			LOGGING_ERROR("NO ENTITY TYPE");
		//		}
		//	}
		//	//If Entity Doesnt Have A Parent
		//	else {
		//		if (ColComp->m_type == physicspipe::EntityType::CIRCLE) {

		//			mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(TransComp->m_position, vector2::Vec2{ 1.f, 1.f }, 0);

		//			mat3x3::Mat3RotDeg(rotateMatrix, TransComp->m_rotation);
		//			mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
		//			mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

		//			debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;


		//			debugTransformation = debugTransformation * mat3x3::Mat3Transform(ColComp->m_OffSet, vector2::Vec2{ ColComp->m_radius * 2.f, ColComp->m_radius * 2.f }, 0);
		//			ColComp->m_collider_Transformation = debugTransformation;
		//		}
		//		else if (ColComp->m_type == physicspipe::EntityType::RECTANGLE) {
		//			mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ TransComp->m_transformation.m_e20 , TransComp->m_transformation.m_e21 }, TransComp->m_scale, 0);

		//			mat3x3::Mat3RotDeg(rotateMatrix, TransComp->m_rotation);
		//			mat3x3::Mat3Translate(translateToOriginMatrix, -debugTransformation.m_e20, -debugTransformation.m_e21);
		//			mat3x3::Mat3Translate(translateBackMatrix, debugTransformation.m_e20, debugTransformation.m_e21);

		//			debugTransformation = translateBackMatrix * rotateMatrix * translateToOriginMatrix * debugTransformation;

		//			debugTransformation = debugTransformation * mat3x3::Mat3Transform(ColComp->m_OffSet, vector2::Vec2{ ColComp->m_Size.m_x , ColComp->m_Size.m_y }, 0);


		//			ColComp->m_collider_Transformation = debugTransformation;
		//		}
		//		else {
		//			LOGGING_ERROR("NO ENTITY TYPE");
		//		}
		//	}
		//}

	}
}
