#pragma once
#include "BaseMessage.h"
#include <unordered_map>

namespace messaging {
	class MessageSystem {
	public:
		using MessageCallback = void (*)(const Message&);
		void m_AddListener(MessageType listenToType, MessageCallback callback);
		void m_SendMessage(const Message&);

	private:
		std::unordered_map<MessageType, std::vector<MessageCallback>> m_listeners;
	};
}