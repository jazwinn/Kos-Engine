#include "../ECS.h"

#include "RenderTextSystem.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Application/Helper.h"

namespace ecs {

	void RenderTextSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		if (std::find_if(m_vecTextComponentPtr.begin(), m_vecTextComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecTextComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecTextComponentPtr.push_back((TextComponent*)ecs->m_ECS_CombinedComponentPool[TYPETEXTCOMPONENT]->m_GetEntityComponent(ID));
		}

	}

	void RenderTextSystem::m_DeregisterSystem(EntityID ID) {

		//search element location for the entity
		size_t IndexID{};
		for (auto& SpriteComponentPtr : m_vecTextComponentPtr) {
			if (SpriteComponentPtr->m_Entity == ID) {
				break;
			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecTextComponentPtr.size() - 1;
		std::swap(m_vecTextComponentPtr[IndexID], m_vecTextComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);

		//popback the vector;
		m_vecTextComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
	}

	void RenderTextSystem::m_Init()
	{
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
		m_SystemSignature.set(TYPETEXTCOMPONENT);
	}

	void RenderTextSystem::m_Update()
	{
		//ECS* ecs = ECS::GetInstance();
		graphicpipe::GraphicsPipe* graphicsPipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
		Helper::Helpers* help =  Helper::Helpers::GetInstance();
		if (m_vecTextComponentPtr.size() != m_vecTransformComponentPtr.size()) {
			std::cout << "Error: Vectors container size does not Match" << std::endl;
			return;
		}

		//loops through all vecoters pointing to component
		for (int n{}; n < m_vecTextComponentPtr.size(); n++) {

			//std::cout << "Update Entity: " << n << std::endl;
			//sprite not need currently
			//SpriteComponent* MovComp = vecSpriteComponentPtr[n];
			TransformComponent* transform = m_vecTransformComponentPtr[n];
			TextComponent* text = m_vecTextComponentPtr[n];
			float red = text->m_red;
			float green = text->m_green;
			float blue = text->m_blue;

			float windowCordianteX = (transform->m_position.m_x + 1) / 2 * help->WindowWidth;
			float windowCordianteY = (transform->m_position.m_y + 1) / 2 * help->WindowHeight;

			graphicsPipe->m_textData.push_back(graphicpipe::TextData{ text->m_text,  windowCordianteX , windowCordianteY, text->m_fontSize, {red, green, blue }});
			//m_funcDrawText("(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));

		}


	}


}

