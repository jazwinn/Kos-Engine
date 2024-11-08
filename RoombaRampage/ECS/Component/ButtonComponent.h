/******************************************************************/
/*!
\file      ColliderComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the button data


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef BUTTONCOM_H
#define BUTTONCOM_H

#include "Component.h"

namespace ecs {


	class ButtonComponent : public Component {

	public:
		vector2::Vec2 m_Position{};
		vector2::Vec2 m_Scale{};
		bool m_IsClick = false;
	};

}




#endif CAMCOM_H