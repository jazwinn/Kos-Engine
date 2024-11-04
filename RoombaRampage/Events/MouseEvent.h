/*!
\file      MouseEvent.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the declaration and definitions of the specialised mouse event classes

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "BaseEvent.h"

namespace events {
	
	class MousePressedEvent : public BaseEvent<MouseEvents> {
	private:
		int m_mouseButton;
	public:
		MousePressedEvent(int givenButton) : BaseEvent<MouseEvents>(MouseEvents::EVENTMOUSEPRESS), m_mouseButton(givenButton) {};
	};

	class MouseReleasedEvent : public BaseEvent<MouseEvents> {
	private:
		int m_mouseButton;
	public:
		MouseReleasedEvent(int givenButton) : BaseEvent<MouseEvents>(MouseEvents::EVENTMOUSERELEASE), m_mouseButton(givenButton){};
	};
}