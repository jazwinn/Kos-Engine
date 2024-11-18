/******************************************************************/
/*!
\file      CollisionSystem.cpp
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 29, 2024
\brief     This file contains the definition of the CollisionSystem class,
		   which is part of the Entity Component System (ECS) framework.

		   The CollisionSystem class is responsible for managing and
		   processing collisions between entities that have both
		   Transform and Collider components. It registers and
		   deregisters entities with collision-related components and
		   updates the system to handle collisions during the game loop.
		   
		   The class extends the ISystem interface, ensuring it adheres
		   to the system management structure within the ECS framework.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "../ECS.h"

#include "CollisionSystem.h"
#include "../ECS/Hierachy.h"
#include "../Physics/Physics.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Debugging/Logging.h"

namespace ecs {

	void CollisionSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecColliderComponentPtr.push_back((ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ID));
			m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void CollisionSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecColliderComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecColliderComponentPtr.size() - 1;

		std::swap(m_vecColliderComponentPtr[IndexID], m_vecColliderComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);

		//popback the vector;
		m_vecColliderComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecNameComponentPtr.pop_back();
	}

	void CollisionSystem::m_Init() {

		m_SystemSignature.set(TYPECOLLIDERCOMPONENT);

	}

	void CollisionSystem::m_Update(const std::string& scene) {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecColliderComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			//std::cout << "Error: Vecotrs container size does not Match" << std::endl;
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}
		
		//TODO find better way to fill up data
		for (int n{}; n < m_vecColliderComponentPtr.size(); n++)
		{
			ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
			ColComp->m_isCollided = false;
		}

		//create physics;
		physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::getInstance();
		for (int n{}; n < m_vecTransformComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";
			
			ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
			TransformComponent* TransComp = m_vecTransformComponentPtr[n];
			NameComponent* NameComp = m_vecNameComponentPtr[n];
			EntityID id = ColComp->m_Entity;

			//skip component not of the scene
			if (ColComp->m_scene != scene) continue;
			

			//if movement component is present, do dynamic collision
			vector2::Vec2 velocity{};
			if (ecs->m_ECS_EntityMap[id].test(TYPERIGIDBODYCOMPONENT)) {

				RigidBodyComponent* rigidComp = static_cast<RigidBodyComponent*>(ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(id));

				 velocity = rigidComp->m_Velocity;
			}

			vector2::Vec2 position{ TransComp->m_transformation.m_e20,TransComp->m_transformation.m_e21 };

			//if (TransComp->m_haveParent) {
			//	EntityID parentID = ecs::Hierachy::m_GetParent(TransComp->m_Entity).value();
			//	//retrieve paretn transforamtion
			//	TransformComponent* rigidComp = static_cast<TransformComponent*>(ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(id));


			//	mat3x3::Mat3x3 transform = mat3x3::Mat3Transform(position, scale, rotation);
			//	transform = rigidComp->m_transformation * transform;

			//	mat3x3::Mat3Decompose(transform, position, scale, rotation);
			//}
			if (TransComp->m_haveParent) {
				EntityID parentID = ecs::Hierachy::m_GetParent(TransComp->m_Entity).value();
				TransformComponent* parentComp{ nullptr };
				for (auto& com : m_vecTransformComponentPtr) {
					if (com->m_Entity == parentID) {
						parentComp = com;
					}
				}
				if (!parentComp) continue;
				mat3x3::Mat3x3 parentTransformation = parentComp->m_transformation;
				mat3x3::Mat3x3 childTransformation = TransComp->m_transformation;

				/*
					Child collider box doesnt rotate in accordance to the parent
					Could be the scale issue
				*/
			
				vector2::Vec2 pos{}, scale{};
				float rot{};
				mat3x3::Mat3Decompose(TransComp->m_transformation, pos, scale, rot);

				if (ColComp->m_type == physicspipe::EntityType::CIRCLE) {
					PhysicsPipeline->m_SendPhysicsData(ColComp->m_radius, pos + ColComp->m_OffSet, scale, velocity, id, NameComp->m_Layer);
				}
				else if (ColComp->m_type == physicspipe::EntityType::RECTANGLE) {
					PhysicsPipeline->m_SendPhysicsData(ColComp->m_Size.m_y, ColComp->m_Size.m_x, rot, pos + ColComp->m_OffSet, scale, velocity, id, NameComp->m_Layer);
				}
				else {
					LOGGING_ERROR("NO ENTITY TYPE");
				}
			}
			else {
				if (ColComp->m_type == physicspipe::EntityType::CIRCLE) {
					PhysicsPipeline->m_SendPhysicsData(ColComp->m_radius, TransComp->m_position + ColComp->m_OffSet, ColComp->m_Size * TransComp->m_scale, velocity, id, NameComp->m_Layer);
				}
				else if (ColComp->m_type == physicspipe::EntityType::RECTANGLE) {
					//why pass same colcomp data type twice?
					PhysicsPipeline->m_SendPhysicsData(ColComp->m_Size.m_y * TransComp->m_scale.m_y, ColComp->m_Size.m_x * TransComp->m_scale.m_x, TransComp->m_rotation, TransComp->m_position + ColComp->m_OffSet, ColComp->m_Size * TransComp->m_scale, velocity, id, NameComp->m_Layer);
				}
				else {
					LOGGING_ERROR("NO ENTITY TYPE");
				}
			}
		}


	
	}
		

}
