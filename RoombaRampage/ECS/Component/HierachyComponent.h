/******************************************************************/
/*!
\file      HeirachyComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the collider data


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef HIERACHYCOM_H
#define HIERACHYCOM_H

#include "Component.h"
#include "../Math/vector2.h"
#include <vector>

namespace ecs {
	/******************************************************************/
	/*!
	\class     ColliderComponent
	\brief     Contains all the shape data
	*/
	/******************************************************************/

	class HierachyComponent : public Component {

	public:

		bool m_hasChild{false};
		bool m_hasParent{true};

		std::vector<ecs::EntityID> m_childIds{};
		ecs::EntityID m_parentId{};

		vector2::Vec2 offset{};

	};

}




#endif HIERACHYCOM_H
