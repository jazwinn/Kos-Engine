/******************************************************************/
/*!
\file      PlayerComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains player data


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef INPUTCOM_H
#define INPUTCOM_H

#include "Component.h"


namespace ecs {

	/******************************************************************/
	/*!
	\class     PlayerComponent
	\brief     Contains data specific to a player
	*/
	/******************************************************************/
	class PlayerComponent : public Component {

	public:

		bool m_Control{true};

		REFLECTABLE(PlayerComponent, m_Control);

	};

}




#endif INPUTCOM_H
