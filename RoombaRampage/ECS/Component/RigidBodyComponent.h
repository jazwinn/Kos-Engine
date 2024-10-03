/******************************************************************/
/*!
\file      RigidBodyComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for rigid body


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef RIGIDCOM_H
#define RIGIDCOM_H

#include "../Math/Vector2.h"
#include "Component.h"


namespace ecs {

	/******************************************************************/
	/*!
	\class     RigidBodyComponent
	\brief     Contains rigid body data
	*/
	/******************************************************************/
	class RigidBodyComponent : public Component {

	public:

		float m_Mass{};

	};

}




#endif RIGIDCOM_H