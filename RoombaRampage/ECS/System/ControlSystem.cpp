/******************************************************************/
/*!
\file      ControlSystem.cpp
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 28, 2024
\brief     This file contains the definition of the ControlSystem class,
		   which is part of the Entity Component System (ECS) framework.

		   The ControlSystem class manages player input and movement control
		   for entities in the ECS framework. It processes input for
		   entities with PlayerComponent and MovementComponent, updating
		   their states based on player commands.

		   The class extends the ISystem interface, ensuring it conforms
		   to the ECS structure, allowing registration, deregistration,
		   initialization, and updating of entities in the system.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../ECS.h"

#include "ControlSystem.h"
#include "../Inputs/Input.h"

namespace ecs {

	void ControlSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecMovementComponentPtr.begin(), m_vecMovementComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecMovementComponentPtr.end()) {
			m_vecPlayerComponentPtr.push_back((PlayerComponent*)ecs->m_ECS_CombinedComponentPool[TYPEPLAYERCOMPONENT]->m_GetEntityComponent(ID));
			//m_vecMovementComponentPtr.push_back((MovementComponent*)ecs->m_ECS_CombinedComponentPool[TYPEMOVEMENTCOMPONENT]->m_GetEntityComponent(ID));
		}


	}

	void ControlSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecPlayerComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecPlayerComponentPtr.size() - 1;

		std::swap(m_vecPlayerComponentPtr[IndexID], m_vecPlayerComponentPtr[IndexLast]);
		//std::swap(m_vecMovementComponentPtr[IndexID], m_vecMovementComponentPtr[IndexLast]);

		//popback the vector;
		m_vecPlayerComponentPtr.pop_back();
		//m_vecMovementComponentPtr.pop_back();
	}

	void ControlSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPEPLAYERCOMPONENT);
		//m_SystemSignature.set(TYPEMOVEMENTCOMPONENT);
		//SystemSignature.set();

	}

	void ControlSystem::m_Update() {

		//ECS* ecs = ECS::GetInstance();

		if (m_vecMovementComponentPtr.size() != m_vecPlayerComponentPtr.size()) {
			//std::cout << "Error: Vecotrs container size does not Match" << std::endl;
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}

		for (int n{}; n < m_vecMovementComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";

			PlayerComponent* playerComp = m_vecPlayerComponentPtr[n];
			MovementComponent* MovComp = m_vecMovementComponentPtr[n];

			if (playerComp->m_Control == false)continue;

			// set direction to always be 0,0
			//MovComp->Direction.x = 0;
			//MovComp->Direction.y = 0;

			vector2::Vec2 NewDirection{};
			if (Input::InputSystem::KeyStateW) {
				NewDirection.m_y += 1;
			}

			if (Input::InputSystem::KeyStateS) {
				NewDirection.m_y += -1;
			}

			if (Input::InputSystem::KeyStateD) {
				NewDirection.m_x += 1;
			}

			if (Input::InputSystem::KeyStateA) {
				NewDirection.m_x += -1;
			}

			if ((NewDirection.m_x != 0) && (NewDirection.m_y != 0)) {
				vector2::Vec2::m_funcVec2Normalize(NewDirection, NewDirection);
				
			}

			MovComp->m_Direction = NewDirection;

		}

	}


}
