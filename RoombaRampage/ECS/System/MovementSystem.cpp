/*!
\file      MovementSystem.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the definition of the MovementSystem class's member functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../ECS.h"

#include "MovementSystem.h"


namespace ecs {

	void MovementSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecMovementComponentPtr.push_back((MovementComponent*)ecs->m_ECS_CombinedComponentPool[TYPEMOVEMENTCOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void MovementSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& MovComponentPtr : m_vecMovementComponentPtr) {
			if (MovComponentPtr->m_Entity == ID) {
				return;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecMovementComponentPtr.size() - 1;
		std::swap(m_vecMovementComponentPtr[IndexID], m_vecMovementComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);

		//popback the vector;
		m_vecMovementComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
	}

	void MovementSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPEMOVEMENTCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);

	}

	void MovementSystem::m_Update() {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecMovementComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			//std::cout << "Error: Vectors container size does not Match" << std::endl;
			LOGGING_ERROR("Error: Vectors container size does not Match");
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecMovementComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";

			MovementComponent* MovComp = m_vecMovementComponentPtr[n];
			TransformComponent* TransComp = m_vecTransformComponentPtr[n];

				vector2::Vec2 Velocity = MovComp->m_Direction * MovComp->m_Speed;

				vector2::Vec2 Displacement = Velocity * ecs->m_DeltaTime;

				TransComp->m_position += Displacement;
		}
	}

}

