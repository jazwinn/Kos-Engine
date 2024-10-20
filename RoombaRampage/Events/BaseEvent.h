#pragma once
#include <functional>
#include <string>
#include <map>
#include <vector>



namespace events {
	//enum class EventType {
	//	EVENTNONE = 0,
	//	EVENTBUTTONPRESS,
	//	EVENTKEYPRESS,EVENTKEYRELEASE,
	//	EVENTMOUSEPRESS,EVENTMOUSERELEASE
	//};

	enum class ButtonEvents{
		EVENTBUTTONPRESS
	};

	enum class KeyEvents {
		EVENTKEYPRESS,
		EVENTKEYRELEASE
	};

	enum class MouseEvents {
		EVENTMOUSEPRESS,
		EVENTMOUSERELEASE
	};


	template<typename T>
	class BaseEvent {
	protected:
		T m_eventType;
		bool m_handled;
	public:
		BaseEvent(T givenType) : m_eventType{ givenType }, m_handled{ false } {}
		virtual ~BaseEvent() = default;
		inline const T m_GetEventType() const { return m_eventType; }
		template<typename U>
		inline U m_ToType()const {
			return static_cast<const U&>(*this);
		}
		virtual bool m_IsHandled() const{ return m_handled; }
	};


	template<typename T>
	class EventDispatcher {
	private:
		std::unordered_map<T, std::vector<std::function<void(const BaseEvent<T>&)>>> m_listeners;
	public:
		void RegisterListener(T givenEvent, std::function<void(const BaseEvent<T>&)> givenCallback) {
			m_listeners[givenEvent].emplace_back(givenCallback);
		}

		void DispatchToListeners(const BaseEvent<T>& givenEvent) {
			if (m_listeners.find(givenEvent.m_GetEventType()) == m_listeners.end()) {
				return;
			}

			for (auto&& currListener : m_listeners.at(givenEvent.m_GetEventType())) {
				if (!givenEvent.m_IsHandled()) {
					currListener(givenEvent);
				}
			}
		}
	};
}