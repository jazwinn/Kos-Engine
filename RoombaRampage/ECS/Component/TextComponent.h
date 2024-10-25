/******************************************************************/
/*!
\file      TextComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for the text component


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef TEXTCOM_H
#define TEXTCOM_H

#include <string>
#include "Component.h"


namespace ecs {

	/******************************************************************/
	/*!
	\class     TextComponet
	\brief     Contains data required to render text
	*/
	/******************************************************************/
	class TextComponent : public Component {

	public:

		std::string m_text{"Insert Text"};
		std::string m_fileName{};
		float m_fontSize{ 1 };

		float m_red{}, m_green{}, m_blue{};

	};

}




#endif TEXTCOM_H
