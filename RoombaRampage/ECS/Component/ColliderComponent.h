/******************************************************************/
/*!
\file      ColliderComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the collider data 


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef BOXCOM_H
#define BOXCOM_H

#include "Component.h"
#include "../Physics/Physics.h"


namespace ecs {
/******************************************************************/
/*!
\class     ColliderComponent
\brief     Contains all the shape data
*/
/******************************************************************/

	class ColliderComponent : public Component {

	public:
		
		vector2::Vec2 m_Size = {1,1};

		vector2::Vec2 m_OffSet{};

		bool m_drawDebug{true};

		physicspipe::EntityType m_type = physicspipe::EntityType::RECTANGLE;

		float m_radius{0.5f};

		bool m_CollisionCheck{ true };


		// no need to serialize or reflect
		float m_isCollided{ false };

		std::vector<EntityID> m_collidedWith{};

		int m_blockedFlag = 0;

		mat3x3::Mat3x3 m_collider_Transformation{};
		 
		REFLECTABLE(ColliderComponent, m_CollisionCheck, m_Size, m_OffSet, m_drawDebug, m_radius, m_type)
	};

}




#endif BOXCOM_H