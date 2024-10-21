#pragma once
#include "BaseEvent.h"

namespace events {
	
	class MousePressedEvent : public BaseEvent<MouseEvents> {
		MousePressedEvent() : BaseEvent<MouseEvents>(MouseEvents::EVENTMOUSEPRESS) {};
	};

	class MouseReleasedEvent : public BaseEvent<MouseEvents> {
		MouseReleasedEvent() : BaseEvent<MouseEvents>(MouseEvents::EVENTMOUSERELEASE) {};
	};
}