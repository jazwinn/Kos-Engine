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
#include "../ECS/Layers.h"


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

		layer::LAYERS m_Layer{ layer::DEFAULT };

		bool m_isPrefab{ false };
		
		bool m_syncPrefab{true};
		std::string m_prefabName{};

		std::string m_entityTag = "Default";

		REFLECTABLE(NameComponent, m_entityName, m_entityTag);
	};

}




#endif NAMECOM_H