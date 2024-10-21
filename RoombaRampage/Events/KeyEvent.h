#pragma once
#include "BaseEvent.h"

namespace events {
	class KeyPressedEvent : public BaseEvent<KeyEvents> {
	public:
		KeyPressedEvent() : BaseEvent<KeyEvents>(KeyEvents::EVENTKEYPRESS){};
	};

	class KeyReleasedEvent : public BaseEvent<KeyEvents> {
	public:
		KeyReleasedEvent() : BaseEvent<KeyEvents>(KeyEvents::EVENTKEYRELEASE) {};
	};
}