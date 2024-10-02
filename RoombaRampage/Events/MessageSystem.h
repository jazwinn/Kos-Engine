/******************************************************************/
/*!
\file      MessagingSystem.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     The main functions of the messaging system (Adding a callbacks to an event type and to send messages.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
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