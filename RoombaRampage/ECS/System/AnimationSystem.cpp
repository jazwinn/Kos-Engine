/******************************************************************/
/*!
\file      AnimationSystem.cpp
\author    
\par       
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
#include "../Graphics/GraphicsPipe.h"
#include "../Asset Manager/AssetManager.h"
#include "AnimationSystem.h"
#include "../Debugging/Logging.h"


namespace ecs {

	void AnimationSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecAnimationComponentPtr.begin(), m_vecAnimationComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecAnimationComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecAnimationComponentPtr.push_back((AnimationComponent*)ecs->m_ECS_CombinedComponentPool[TYPEANIMATIONCOMPONENT]->m_GetEntityComponent(ID));
			m_vecSpriteComponentPtr.push_back((SpriteComponent*)ecs->m_ECS_CombinedComponentPool[TYPESPRITECOMPONENT]->m_GetEntityComponent(ID));

		}
	}

	void AnimationSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecAnimationComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecAnimationComponentPtr.size() - 1;

		std::swap(m_vecAnimationComponentPtr[IndexID], m_vecAnimationComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
		std::swap(m_vecSpriteComponentPtr[IndexID], m_vecSpriteComponentPtr[IndexLast]);

		//popback the vector;
		m_vecAnimationComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
		m_vecSpriteComponentPtr.pop_back();

	}

	void AnimationSystem::m_Init() {

		m_SystemSignature.set(TYPEANIMATIONCOMPONENT);
		m_SystemSignature.set(TYPESPRITECOMPONENT);

	}

	void AnimationSystem::m_Update(const std::string& scene) {

		ECS* ecs = ECS::m_GetInstance();
		graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();

		if (m_vecAnimationComponentPtr.size() != m_vecTransformComponentPtr.size() && m_vecAnimationComponentPtr.size() != m_vecSpriteComponentPtr.size()) {
			//std::cout << "Error: Vecotrs container size does not Match" << std::endl;
			LOGGING_ERROR("Error: Vecotrs container size does not Match");
			return;
		}


		//graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
		for (int n{}; n < m_vecAnimationComponentPtr.size(); n++) {

			AnimationComponent* AniComp = m_vecAnimationComponentPtr[n];
			SpriteComponent* SpriteComp = m_vecSpriteComponentPtr[n];

			//skip component not of the scene
			if (AniComp->m_scene != scene) continue;

			if (m_vecAnimationComponentPtr[n]->m_isAnimating && m_vecAnimationComponentPtr[n]->m_framesPerSecond)
			{
				m_vecAnimationComponentPtr[n]->m_frameTimer += ecs->m_DeltaTime;

				float frameTime = 1.f / m_vecAnimationComponentPtr[n]->m_framesPerSecond;
				int ids = assetmanager->m_imageManager.m_imageMap.find(SpriteComp->m_imageFile)->second.m_imageID;
				//float spriteTotalTime = frameTime * pipe->m_imageData[ids].m_stripCount;
				float spriteTotalTime = frameTime * m_vecAnimationComponentPtr[n]->m_stripCount;
				if (m_vecAnimationComponentPtr[n]->m_frameTimer > spriteTotalTime)
				{
					AniComp->m_frameTimer = 0;
				}
				if (frameTime < spriteTotalTime)
				{
					AniComp->m_frameNumber = static_cast<int>(AniComp->m_frameTimer / frameTime);
				}
				
				//float totalFrameTime = m_frameTimer * m_imageData[m_modelData[n].m_textureID].m_stripCount;
				//float frameTime = static_cast<float>(fmod(m_modelData[n].m_animationTimer, totalFrameTime));
				//int frameNumber = static_cast<int>(frameTime / m_frameTime);
				//std::cout << AniComp->m_frameNumber << std::endl;
			}
			else
			{
				m_vecAnimationComponentPtr[n]->m_frameTimer = 0;
			}
			

		}


	}


}
