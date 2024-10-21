/******************************************************************/
/*!
\file      MovementComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for the movement componetn


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef MOVECOM_H
#define MOVECOM_H

#include "Component.h"

namespace ecs {
	/******************************************************************/
	/*!
	\class     MovementComponent
	\brief    Contains the speed and direction data
	*/
	/******************************************************************/
	class MovementComponent : public Component {

	public:
		float m_Speed{1};
		vector2::Vec2 m_Direction{0,0};
	};
}

#endif MOVECOM_H
