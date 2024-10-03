/******************************************************************/
/*!
\file      RenderDebugSystem.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains class for the DebugDrawingSystem


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef DEBUGDRAWSYS_H
#define DEBUGDRAWSYS_H

#include "System.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/ColliderComponent.h"

#include "../ECS/ECSList.h"

namespace ecs {

	class DebugDrawingSystem : public ISystem {

	public:
		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update() override;

	private:
		//Storage to point to components
		std::vector<TransformComponent*> m_vecTransformComponentPtr;
		std::vector<ColliderComponent*> m_vecColliderComponentPtr;

	};

}



#endif DEBUGDRAWSYS_H
