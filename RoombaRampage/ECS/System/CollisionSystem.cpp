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
#include "../ECS.h"

#include "CollisionSystem.h"
#include "../Physics/Physics.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Debugging/Logging.h"
#include <algorithm>

namespace ecs {

	void CollisionSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecColliderComponentPtr.push_back((ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ID));

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

		//popback the vector;
		m_vecColliderComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();

	}

	void CollisionSystem::m_Init() {

		m_SystemSignature.set(TYPECOLLIDERCOMPONENT);

	}

	void CollisionSystem::m_Update() {

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
		physicspipe::Physics PhysicsPipeline;
		//graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();

		for (int n{}; n < m_vecTransformComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";
			
			ColliderComponent* ColComp = m_vecColliderComponentPtr[n];
			TransformComponent* TransComp = m_vecTransformComponentPtr[n];

			EntityID id = ColComp->m_Entity;
			

			//if movement component is present, do dynamic collision
			vector2::Vec2 velocity{};
			if (ecs->m_ECS_EntityMap[id].test(TYPEMOVEMENTCOMPONENT)) {

				MovementComponent* MovComp = static_cast<MovementComponent*>(ecs->m_ECS_CombinedComponentPool[TYPEMOVEMENTCOMPONENT]->m_GetEntityComponent(id));

				 velocity = MovComp->m_Speed * MovComp->m_Direction;
			}

			if (ColComp->m_type == physicspipe::EntityType::CIRCLE) {
				PhysicsPipeline.m_SendPhysicsData(ColComp->m_radius, TransComp->m_position + ColComp->m_OffSet, ColComp->m_Size, velocity, id);
			}
			else if (ColComp->m_type == physicspipe::EntityType::RECTANGLE) {
				PhysicsPipeline.m_SendPhysicsData(ColComp->m_Size.m_y , ColComp->m_Size.m_x, TransComp->m_position + ColComp->m_OffSet, ColComp->m_Size, velocity, id);
			}
			else {
				LOGGING_ERROR("NO ENTITY TYPE");
			}

		}

		//check for collision
		if (m_vecColliderComponentPtr.size() > 0) {
			PhysicsPipeline.m_CollisionCheck(ecs->m_DeltaTime);
		}
	
	}
		

}
