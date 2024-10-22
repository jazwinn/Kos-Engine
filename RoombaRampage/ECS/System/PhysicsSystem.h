/*!
\file      PhysicsSystem.h
\author    
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the declaration of the PhysicsSystem class

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef MOVESYS_H
#define MOVESYS_H

#include "System.h"
#include "../ECS/Component/TransformComponent.h"
#include "../ECS/Component/RigidBodyComponent.h"

namespace ecs {

	class PhysicsSystem : public ISystem {

	public:

		void m_RegisterSystem(EntityID) override;

		void m_DeregisterSystem(EntityID) override;

		void m_Init() override;

		void m_Update() override;

	private:
		//Storage to point to components
		std::vector<TransformComponent*> m_vecTransformComponentPtr;
		std::vector<RigidBodyComponent*> m_vecRigidBodyComponentPtr;
	};

}



#endif MOVESYS_H