/*!
\file      ActionEvent.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the declaration and definitions of the specialised action event classes for undo/redo

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "BaseEvent.h"
#include "../Math/vector2.h"
#include "../Math/Mat3x3.h"

namespace events {
	class TranslateComponentChanged : public BaseEvent<Actions> {
	private:
		int m_mouseButton;
	public:
		TranslateComponentChanged(int givenButton) : BaseEvent<MouseEvents>(Actions::TRANSLATECOMP), m_mouseButton(givenButton) {};
	};
}
