#include "MessageSystem.h"

namespace messaging {
	/**
	 \brief		this function adds a callback to a message type to the unordered map held by the message system
	 **/
	void MessageSystem::m_AddListener(MessageType listenToType, MessageCallback callback) {
		m_listeners[listenToType].emplace_back(callback);
	}

	/**
	 \brief		this function broadcasts/sends a message to all listeners of a specified message type
	 **/
	void MessageSystem::m_SendMessage(const Message& givenMsg) {
		auto iter = m_listeners.find(givenMsg.m_GetType());
		if (iter != m_listeners.end()) {
			for (auto& callback : m_listeners[givenMsg.m_GetType()]) {
				callback(givenMsg);
			}
		}
	}
}