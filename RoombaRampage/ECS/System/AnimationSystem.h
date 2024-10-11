/******************************************************************/
/*!
\file      AnimationSystem.h
\author    
\par       
\date      Oct 02, 2024
\brief     This file contains class for collision response system


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef ANIMATIONSYS_H
#define ANIMATIONSYS_H

#include "System.h"
#include "../ECS/Component/AnimationComponent.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/SpriteComponent.h"

#include "../ECS/ECSList.h"

namespace ecs {

	class AnimationSystem : public ISystem {

	public:
		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update() override;

	private:
		//Storage to point to components
		std::vector<TransformComponent*> m_vecTransformComponentPtr;
		std::vector<AnimationComponent*> m_vecAnimationComponentPtr;
		std::vector<SpriteComponent*>	 m_vecSpriteComponentPtr;
		//std::vector<MovementComponent*> m_vecMovementComponentPtr;

	};

}



#endif ANIMATIONSYS_H
