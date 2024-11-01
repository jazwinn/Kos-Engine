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
#include "../Asset Manager/AssetManager.h"

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
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();

		if (m_vecSpriteComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			std::cout << "Error: Vectors container size does not Match" << std::endl;
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecSpriteComponentPtr.size(); n++) {

			TransformComponent* transform = m_vecTransformComponentPtr[n];
			SpriteComponent* sprite = m_vecSpriteComponentPtr[n];

			ECS* ecs = ECS::m_GetInstance();

			if (assetmanager->m_imageManager.m_imageMap.find(sprite->m_imageFile) == assetmanager->m_imageManager.m_imageMap.end()) continue;

			unsigned int textureid = assetmanager->m_imageManager.m_imageMap.find(sprite->m_imageFile)->second.m_imageID;
			
			if (ecs->m_ECS_EntityMap[sprite->m_Entity].test(TYPEANIMATIONCOMPONENT)) 
			{
				AnimationComponent* animation = (AnimationComponent*)ecs->m_ECS_CombinedComponentPool[TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(sprite->m_Entity);
				
				graphicsPipe->m_modelData.push_back({ glm::mat3{transform->m_transformation.m_e00,transform->m_transformation.m_e01,transform->m_transformation.m_e02,
																transform->m_transformation.m_e10,transform->m_transformation.m_e11, transform->m_transformation.m_e12,
															transform->m_transformation.m_e20, transform->m_transformation.m_e21, transform->m_transformation.m_e22} ,
															textureid, animation->m_frameNumber, sprite->m_layer, glm::vec4{sprite->m_color.m_x,sprite->m_color.m_y, sprite->m_color.m_z, sprite->m_alpha} });
			}
			else
			{
				graphicsPipe->m_modelData.push_back({ glm::mat3{transform->m_transformation.m_e00,transform->m_transformation.m_e01,transform->m_transformation.m_e02,
																transform->m_transformation.m_e10,transform->m_transformation.m_e11, transform->m_transformation.m_e12,
															transform->m_transformation.m_e20, transform->m_transformation.m_e21, transform->m_transformation.m_e22},textureid, 0, sprite->m_layer 
															, glm::vec4{sprite->m_color.m_x,sprite->m_color.m_y, sprite->m_color.m_z, sprite->m_alpha} });
			}
			
		}


	}


}

