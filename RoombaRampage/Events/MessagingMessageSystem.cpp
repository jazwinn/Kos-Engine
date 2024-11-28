#include "../Config/pch.h"
#include "MessagingMessageSystem.h"

namespace messaging {

	void MessageSystem::m_AddListener(MessageType listenToType, MessageCallback callback) {
		m_listeners[listenToType].emplace_back(callback);
	}


	void MessageSystem::m_SendMessage(const Message& givenMsg) {
		auto iter = m_listeners.find(givenMsg.m_GetType());
		if (iter != m_listeners.end()) {
			for (auto& callback : m_listeners[givenMsg.m_GetType()]) {
				callback(givenMsg);
			}
		}
	}
}