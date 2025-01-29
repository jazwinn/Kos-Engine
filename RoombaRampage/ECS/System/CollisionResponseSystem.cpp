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
		
		for (int n{}; n < m_vecRigidBodyComponentPtr.size(); n++)
		{

			RigidBodyComponent* rigidComp = m_vecRigidBodyComponentPtr[n];
			ColliderComponent* ColComp = m_vecColliderComponentPtr[n];

			ColComp->m_collidedWith.clear();
			ColComp->m_blockedFlag = -1;
			//skip component not of the scene
			if (rigidComp->m_scene != scene) continue;

			const EntityID check_ID = ColComp->m_Entity;
			const auto& iterator = std::find_if(vecCollisionEntityPair.begin(), vecCollisionEntityPair.end(), [check_ID](const auto pair) { return (static_cast<int>(check_ID) == pair.first->m_ID); });
			if (iterator != vecCollisionEntityPair.end()) {
				ColComp->m_collidedWith.push_back(iterator->second->m_ID);
				ColComp->m_isCollided = true;
				ColComp->m_blockedFlag = iterator->first->m_collisionFlags;
			}

		}
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
