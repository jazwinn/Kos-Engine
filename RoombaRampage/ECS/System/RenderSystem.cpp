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
		GraphicsPipe* graphicsPipe = GraphicsPipe::funcGetInstance();

		if (m_vecSpriteComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			std::cout << "Error: Vectors container size does not Match" << std::endl;
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecSpriteComponentPtr.size(); n++) {

			//std::cout << "Update Entity: " << n << std::endl;
			//sprite not need currently
			//SpriteComponent* MovComp = vecSpriteComponentPtr[n];
			TransformComponent* transform = m_vecTransformComponentPtr[n];
			SpriteComponent* sprite = m_vecSpriteComponentPtr[n];
	

			graphicsPipe->modelData.push_back({ transform->m_rotation, glm::vec2{transform->m_scale.m_x, transform->m_scale.m_y}, glm::vec3{transform->m_position.m_x,transform->m_position.m_y, 0} ,sprite->m_imageID, 0 });

			
			
		}


	}


}

