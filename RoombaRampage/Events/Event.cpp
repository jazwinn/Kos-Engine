#include "../Config/pch.h"
#include "Event.h"


namespace Event {

	void EventSystem::SubscribeEvent(EventType TypeEvent, EventListener listener) {

		Listeners[TypeEvent].push_back(listener);

	}

	void EventSystem::TriggerEvent(EventType TypeEvent) {

		if (Listeners.find(TypeEvent) != Listeners.end()) {
			for (auto& Listener : Listeners[TypeEvent]) {
				Listener();
			}
		}
	}


}