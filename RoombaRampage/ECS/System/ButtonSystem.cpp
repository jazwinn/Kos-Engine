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

#include "ButtonSystem.h"
#include "../Inputs/Input.h"
#include "../Application/Helper.h"

namespace ecs {
	void ButtonSystem::m_RegisterSystem(EntityID ID) {
		ECS* ecs = ECS::m_GetInstance();

		//Checks if system already has stored the entity

		if (std::find_if(m_vecButtonComponentPtr.begin(), m_vecButtonComponentPtr.end(), [ID](const auto& obj) { return obj->m_Entity == ID; })
			== m_vecButtonComponentPtr.end()) {
			m_vecTransformComponentPtr.push_back((TransformComponent*)ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID));
			m_vecButtonComponentPtr.push_back((ButtonComponent*)ecs->m_ECS_CombinedComponentPool[TYPEBUTTONCOMPONENT]->m_GetEntityComponent(ID));
		}
	}

	void ButtonSystem::m_DeregisterSystem(EntityID ID) {
		//search element location for the entity
		size_t IndexID{};
		for (auto& ComponentPtr : m_vecButtonComponentPtr) {
			if (ComponentPtr->m_Entity == ID) {
				break;
 			}
			IndexID++;
		}

		//index to the last element
		size_t IndexLast = m_vecButtonComponentPtr.size() - 1;

		std::swap(m_vecButtonComponentPtr[IndexID], m_vecButtonComponentPtr[IndexLast]);
		std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);

		//popback the vector;
		m_vecButtonComponentPtr.pop_back();
		m_vecTransformComponentPtr.pop_back();
	}

	void ButtonSystem::m_Init() {
		m_SystemSignature.set(TYPEBUTTONCOMPONENT);
		m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
	}

	void ButtonSystem::m_Update() {
		ECS* ecs = ECS::m_GetInstance();
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		if (m_vecTransformComponentPtr.size() != m_vecButtonComponentPtr.size()) {
			LOGGING_ERROR("Error: Vectors container size does not Match");
			return;
		}

		//update box
		float mouseX = Input::InputSystem::MousePosition.m_x;
		float mouseY = Input::InputSystem::MousePosition.m_y;
		float minX, maxX, minY, maxY;

		if (Input::InputSystem::KeyStateLMB && m_vecButtonComponentPtr.size()) {
			for (int i = 0; i < m_vecButtonComponentPtr.size(); ++i) {
				TransformComponent* transform = m_vecTransformComponentPtr[i];
				ButtonComponent* button = m_vecButtonComponentPtr[i];
				float windowCordinateX = (transform->m_position.m_x + 0.5 )  * help->m_windowWidth;
				float windowCordinateY = (transform->m_position.m_y + 1) / 2 * help->m_windowHeight;
				button->m_Position = { windowCordinateX, windowCordinateY };
				button->m_Scale = { 50,50 };
				minX = button->m_Position.m_x - (button->m_Scale.m_x / 2.f);
				maxX = button->m_Position.m_x + (button->m_Scale.m_x / 2.f);
				minY = button->m_Position.m_y - (button->m_Scale.m_y / 2.f);
				maxY = button->m_Position.m_y + (button->m_Scale.m_y / 2.f);
				std::cout << "Mouse position ";
				std::cout << mouseX << " " << mouseY << std::endl;
				std::cout << "Window position ";
				std::cout << help->m_windowWidth << " " << help->m_windowHeight << std::endl;
				std::cout << "object position ";
				std::cout << transform->m_position.m_x << " " << transform->m_position.m_y << std::endl;
				std::cout << "button position ";
		 		std::cout << button->m_Position.m_x << " " << button->m_Position.m_y << std::endl;
				std::cout << minX << " " << minY << " " << maxX << " " << maxY << std::endl;
				if ((minX <= mouseX && mouseX <= maxX) && (minY <= mouseY && mouseY <= maxY)) {
					button->m_IsClick = true;
					std::cout << " clicked " << std::endl;
				}
			}
		}

		for (int i = 0; i < m_vecButtonComponentPtr.size(); ++i) {
			ButtonComponent* button = m_vecButtonComponentPtr[i];
			if (button->m_IsClick) {
				std::cout << "clicked" << std::endl;
				button->m_IsClick = false;
			}
		}
	}
}
 