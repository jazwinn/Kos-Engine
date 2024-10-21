#pragma once
#include "BaseEvent.h"

namespace events {
	class ButtonPressEvent : public BaseEvent<ButtonEvents> {
	private:
		int m_button;
	public:
		ButtonPressEvent(int givenButton) : BaseEvent<ButtonEvents>(ButtonEvents::EVENTBUTTONPRESS), m_button{givenButton} {}
		int m_GetButton() { return m_button; }
	};
}