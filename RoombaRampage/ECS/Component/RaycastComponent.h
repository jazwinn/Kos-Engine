/******************************************************************/
/*!
\file      RaycastComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the animation data


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef RAYCOM_H
#define RAYCOM_H

#include "Component.h"
#include "../ECS/Layers.h"

namespace ecs {



	class RaycastComponent : public Component
	{
	public:

		class Raycast {
		public:
			bool m_isRaycasting{};

			float m_distance{}; // distance between target and entity pos

			vector2::Vec2 m_targetPosition{};

			//std::vector<layer::LAYERS> m_Layers;

			// need not reflect or serialize after this line

			bool m_isHit{};// true if collide with anything in between line

			float m_hitposition{};

			REFLECTABLE(Raycast, m_isRaycasting, m_targetPosition);
		};
		
		std::vector<Raycast> m_raycast{};

		std::vector<int> m_test;;

		REFLECTABLE(RaycastComponent, m_raycast, m_test);

	};

}

#endif