/*!
\file      PhysicsSystem.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the definition of the PhysicsSystem class's member functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../ECS.h"

#include "PhysicsSystem.h"


namespace ecs {

	void PhysicsSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecRigidBodyComponentPtr.push_back((RigidBodyComponent*)ecs->m_ECS_CombinedComponentPool[TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void PhysicsSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& RbComponentPtr : m_vecRigidBodyComponentPtr) {
			if (RbComponentPtr->m_Entity == ID) {
				return;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecRigidBodyComponentPtr.size() - 1;
		std::swap(m_vecRigidBodyComponentPtr[IndexID], m_vecRigidBodyComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);

		//popback the vector;
		m_vecRigidBodyComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
	}

	void PhysicsSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPERIGIDBODYCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);

	}

	void PhysicsSystem::m_Update() {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecRigidBodyComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			//std::cout << "Error: Vectors container size does not Match" << std::endl;
			LOGGING_ERROR("Error: Vectors container size does not Match");
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecRigidBodyComponentPtr.size(); n++) {
			//std::cout << "Entity: " << n << "Movement System is getting Updated";

			RigidBodyComponent* MovComp = m_vecRigidBodyComponentPtr[n];
			TransformComponent* TransComp = m_vecTransformComponentPtr[n];
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>//
				//vector2::Vec2 Velocity = MovComp->m_Direction * MovComp->m_Speed;

				//vector2::Vec2 Displacement = Velocity * ecs->m_DeltaTime;

				//TransComp->m_position += Displacement;
		}
	}

}

