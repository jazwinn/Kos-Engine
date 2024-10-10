/******************************************************************/
/*!
\file      HierachySystem.cpp
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
#include "../ECS.h"

#include "HierachySystem.h"
#include <unordered_set>


namespace ecs {

	void HierachySystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecHeirachyComponentPtr.begin(), m_vecHeirachyComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecHeirachyComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecHeirachyComponentPtr.push_back((HierachyComponent*)ecs->m_ECS_CombinedComponentPool[TYPEHIERACHYCOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void HierachySystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecHeirachyComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecHeirachyComponentPtr.size() - 1;

		std::swap(m_vecHeirachyComponentPtr[IndexID], m_vecHeirachyComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);

		//popback the vector;
		m_vecHeirachyComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
	}

	void HierachySystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPEHIERACHYCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		//SystemSignature.set();

	}

	void HierachySystem::m_Update() {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecHeirachyComponentPtr.size() != m_vecHeirachyComponentPtr.size()) {
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}


		for (int n{}; n < m_vecHeirachyComponentPtr.size(); n++)
		{
			HierachyComponent* hierachyComp = m_vecHeirachyComponentPtr[n];
			TransformComponent* transformComp = m_vecTransformComponentPtr[n];


			if (hierachyComp->m_hasParent) {

				




			}



		}

	}


}
