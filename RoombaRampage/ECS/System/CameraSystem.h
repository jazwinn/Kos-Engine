/******************************************************************/
/*!
\file      CameraSystem.h
\author    
\par       
\date      Oct 02, 2024
\brief     This file contains class for the DebugDrawingSystem


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef CAMERASYS_H
#define CAMERASYS_H

#include "System.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/CameraComponent.h"

#include "../ECS/ECSList.h"

namespace ecs {

	class CameraSystem : public ISystem {

	public:
		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update() override;

	private:
		//Storage to point to components
		std::vector<TransformComponent*> m_vecTransformComponentPtr;
		std::vector<CameraComponent*> m_vecCameraComponentPtr;

	};

}



#endif CAMERASYS_H

