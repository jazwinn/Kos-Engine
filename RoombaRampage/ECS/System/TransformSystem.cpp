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

#include "../Config/pch.h"
#include "../ECS.h"

#include "TransformSystem.h"
#include "../ECS/Hierachy.h"
<<<<<<< HEAD
#include "../Graphics/GraphicsCamera.h"
=======
#include <unordered_set>
>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128


namespace ecs {

	void TransformSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void TransformSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecTransformComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecTransformComponentPtr.size() - 1;

		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);

		//popback the vector;
		m_vecTransformComponentPtr.pop_back();
	}

	void TransformSystem::m_Init() {

		// requires both movement component and transform component
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		//SystemSignature.set();

	}

	void TransformSystem::m_Update(const std::string& scene) {

		//ECS* ecs = ECS::m_GetInstance();


		for (int n{}; n < m_vecTransformComponentPtr.size(); n++)
		{
			TransformComponent* transformComp = m_vecTransformComponentPtr[n];

<<<<<<< HEAD
			//skip component not of the scene
			if (transformComp->m_scene != scene) continue;
=======

>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128

			transformComp->m_transformation = mat3x3::Mat3Transform(transformComp->m_position, transformComp->m_scale, transformComp->m_rotation);

			if (!transformComp->m_haveParent) {
				continue;
			}
			//get parents coordinate
			if (!ecs::Hierachy::m_GetParent(transformComp->m_Entity).has_value()) {
				// no parnet
				return;
			}

			EntityID parentID = ecs::Hierachy::m_GetParent(transformComp->m_Entity).value();
			TransformComponent* parentComp{nullptr};
			for (auto& com : m_vecTransformComponentPtr) {
				if (com->m_Entity == parentID) {
					parentComp = com;
				}
			}
			if (!parentComp) continue;

			//mat3x3::Mat3Translate(transformComp->m_transformation, 1.f, 1.f);
			//calculate parent transformation matrix

			//
			mat3x3::Mat3x3 parentTransformation = parentComp->m_transformation;


			transformComp->m_transformation = parentTransformation * transformComp->m_transformation;


		}

	}


}
