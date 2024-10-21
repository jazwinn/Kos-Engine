#include "EventHandler.h"

namespace events {
	std::unique_ptr<EventHandler> EventHandler::m_instance = nullptr;

	EventHandler* EventHandler::m_GetDispatchInstance() {
		if (!m_instance) {
			m_instance = std::make_unique<EventHandler>();
		}
		return m_instance.get();
	}
}