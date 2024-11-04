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

	class AudioFromImgui : public BaseEvent<ButtonEvents> {
	private:
		std::string m_file;
	public:
		AudioFromImgui(std::string givenFile) : BaseEvent<ButtonEvents>(ButtonEvents::EVENTAUDIOFROMIMGUI), m_file{ givenFile } {}
		std::string m_GetFile() { return m_file; }
	};
}