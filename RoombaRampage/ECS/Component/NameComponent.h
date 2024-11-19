/******************************************************************/
/*!
\file      NameComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for the name componetn


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef NAMECOM_H
#define NAMECOM_H

#include "Component.h"


namespace ecs {

	/******************************************************************/
	/*!
	\class     NameComponent
	\brief    Contains the entity's name
	*/
	/******************************************************************/
	class NameComponent : public Component {

	public:

		std::string m_entityName;

<<<<<<< HEAD
		layer::LAYERS m_Layer{ layer::DEFAULT };

		bool m_isPrefab{ false };
		
		bool m_syncPrefab{true};
		std::string m_prefabName{};

		std::string m_entityTag = "Default";

		REFLECTABLE(NameComponent, m_entityName, m_entityTag);
=======
>>>>>>> 2614f36e3dde51625ed71ac1889d9f61bb456128
	};

}




#endif NAMECOM_H