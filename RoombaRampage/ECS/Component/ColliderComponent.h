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

#include "../Math/Vector2.h"
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

		//layer::LAYERS m_CollideLayer{layer::DEFAULT};

		// Current no implementation
		physicspipe::EntityType m_type = physicspipe::EntityType::RECTANGLE;

		float m_radius{0.5f};

		float m_isCollided{ false };


		REFLECTABLE(ColliderComponent, m_Size, m_OffSet, m_drawDebug, m_radius, m_type)
	};

}




#endif BOXCOM_H