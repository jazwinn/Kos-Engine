/******************************************************************/
/*!
\file      RenderSystem.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains class for the Render System

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef RENDERSYS_H
#define RENDERSYS_H


#include "System.h"
#include "../ECS/ECSList.h"
#include <glew.h>
#include "glfw3.h"
#include <gtc/type_ptr.hpp>

namespace ecs{

	class RenderSystem : public ISystem {

	private:


	public:

		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update(const std::string&) override;


	private:
		//Storage to point to components
		std::vector<TransformComponent*> m_vecTransformComponentPtr;
		std::vector<SpriteComponent*> m_vecSpriteComponentPtr;

	};

}



#endif