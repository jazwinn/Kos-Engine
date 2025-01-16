/******************************************************************/
/*!
\file      ScriptComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for rigid body


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef SCRIPTCOM_H
#define SCRIPTCOM_H

#include "Component.h"
#include <mono/metadata/object.h>

namespace ecs {

	/******************************************************************/
	/*!
	\class     ScriptComponent
	\brief     Contains rigid body data
	*/
	/******************************************************************/
	class ScriptComponent : public Component {

	public:

		std::vector<std::string> m_scripts;

		std::map<std::string,MonoObject*> m_scriptInstances;

		bool m_isEnabled{ true };

		// need not serialize after this line

		bool m_isStart{ false };


		REFLECTABLE(ScriptComponent, m_scripts)

	};

}




#endif SCRIPTCOM_H