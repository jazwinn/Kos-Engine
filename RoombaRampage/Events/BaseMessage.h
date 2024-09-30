#pragma once
#include <string>

namespace messaging {
	enum class MessageType {
		NONE, ENTITYSPAWN, AUDIOPLAY
	};

	class Message {
	public:
		Message(MessageType givenType, const std::string& givenData) : m_type(givenType), m_data(givenData){}
		const MessageType& m_GetType() const { return m_type; }
		const std::string& m_GetData() const { return m_data; }

	private:
		MessageType m_type;
		std::string m_data;
	};
}