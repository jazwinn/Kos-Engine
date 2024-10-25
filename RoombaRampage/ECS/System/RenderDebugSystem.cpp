/******************************************************************/
/*!
\file      RenderDebugSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the RenderDebugSystem
		   class. It passes the debauging data into the graphics 
		   pipeline for it to be rendered.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../ECS.h"

#include "RenderDebugSystem.h"
#include "../Physics/Physics.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"
#include "../Math/Mat3x3.h"
#include "../Math/vector2.h"

namespace ecs {

	void DebugDrawingSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecColliderComponentPtr.push_back((ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void DebugDrawingSystem::m_DeregisterSystem(EntityID ID) {
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

	void DebugDrawingSystem::m_Init() {

		m_SystemSignature.set(TYPECOLLIDERCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);

	}

	void DebugDrawingSystem::m_Update() {

		ECS* ecs = ECS::m_GetInstance();

		if (m_vecTransformComponentPtr.size() != m_vecColliderComponentPtr.size()) {
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}
	

		graphicpipe::GraphicsPipe * graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		for (int n{}; n < m_vecTransformComponentPtr.size(); n++) {

			TransformComponent* transform = m_vecTransformComponentPtr[n];
			if (ecs->m_ECS_EntityMap[transform->m_Entity].test(TYPECOLLIDERCOMPONENT))
			{
				ColliderComponent* collider = (ColliderComponent*)ecs->m_ECS_CombinedComponentPool[TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(transform->m_Entity);

				if (collider->m_drawDebug)
				{
					mat3x3::Mat3x3 debugTransformation = mat3x3::Mat3Transform(vector2::Vec2{ transform->m_transformation.m_e20 + collider->m_OffSet.m_x, transform->m_transformation.m_e21 + collider->m_OffSet.m_y }, vector2::Vec2{ collider->m_Size.m_x, collider->m_Size.m_y }, transform->m_rotation);

					graphicsPipe->m_debugBoxData.push_back({ glm::mat3{debugTransformation.m_e00,debugTransformation.m_e01,debugTransformation.m_e02,
																	debugTransformation.m_e10,debugTransformation.m_e11, debugTransformation.m_e12,
																debugTransformation.m_e20, debugTransformation.m_e21, debugTransformation.m_e22} ,
															collider->m_isCollided, 0 });
				}
			}

			
		}
	}


}
