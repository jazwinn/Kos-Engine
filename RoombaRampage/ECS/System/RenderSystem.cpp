/******************************************************************/
/*!
\file      RenderSystem.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the defination of the RenderSystem
		   class. It passes the transform data into the graphics
		   pipeline for it to be rendered.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../ECS.h"

#include "RenderSystem.h"
#include "../ECS/Component/SpriteComponent.h"
#include "../Graphics/GraphicsPipe.h"

namespace ecs {

	void RenderSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTransformComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecSpriteComponentPtr.push_back((SpriteComponent*)ecs->m_ECS_CombinedComponentPool[TYPESPRITECOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void RenderSystem::m_DeregisterSystem(EntityID ID) {

		//search element location for the entity
		size_t IndexID{};
		for (auto& SpriteComponentPtr : m_vecSpriteComponentPtr) {
			if (SpriteComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecSpriteComponentPtr.size() - 1;
		std::swap(m_vecSpriteComponentPtr[IndexID], m_vecSpriteComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);

		//popback the vector;
		m_vecSpriteComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
	}

	void RenderSystem::m_Init()
	{
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		m_SystemSignature.set(TYPESPRITECOMPONENT);
	}

	void RenderSystem::m_Update()
	{
		//ECS* ecs = ECS::GetInstance();
		graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();

		if (m_vecSpriteComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			std::cout << "Error: Vectors container size does not Match" << std::endl;
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecSpriteComponentPtr.size(); n++) {

			TransformComponent* transform = m_vecTransformComponentPtr[n];
			SpriteComponent* sprite = m_vecSpriteComponentPtr[n];

			ECS* ecs = ECS::m_GetInstance();

			if (ecs->m_ECS_EntityMap[sprite->m_Entity].test(TYPEANIMATIONCOMPONENT)) 
			{
				AnimationComponent* animation = (AnimationComponent*)ecs->m_ECS_CombinedComponentPool[TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(sprite->m_Entity);
				if (animation->m_isAnimating)
				{
					animation->m_frameTimer += ecs->m_DeltaTime;
				}
				else
				{
					animation->m_frameTimer = 0.f;
				}
				graphicsPipe->m_modelData.push_back({ transform->m_rotation, glm::vec2{transform->m_scale.m_x, transform->m_scale.m_y}, glm::vec2{transform->m_position.m_x,transform->m_position.m_y} ,sprite->m_imageID, animation->m_frameTimer, 0 });
			}
			else
			{
				graphicsPipe->m_modelData.push_back({ transform->m_rotation, glm::vec2{transform->m_scale.m_x, transform->m_scale.m_y}, glm::vec2{transform->m_position.m_x,transform->m_position.m_y} ,sprite->m_imageID, 0, 0 });
			}
			
		}


	}


}

