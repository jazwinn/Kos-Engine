#pragma once
#include "MouseEvent.h"
#include "KeyEvent.h"
#include "ButtonEvent.h"
#include <memory>



namespace events {
	class EventHandler {
	public:
		EventHandler() : m_keyDispatch(), m_mouseDispatch(),m_buttonDispatch() {}
		EventDispatcher<KeyEvents> m_keyDispatch;
		EventDispatcher<MouseEvents> m_mouseDispatch;
		EventDispatcher<ButtonEvents> m_buttonDispatch;
		static EventHandler* m_GetDispatchInstance();

	private:
		static std::unique_ptr<EventHandler> m_instance;
	};
#define REGISTER_BUTTON_LISTENER(eventType, func, arg) events::EventHandler::m_GetDispatchInstance()->m_buttonDispatch.RegisterListener(eventType, std::bind(&func, arg, std::placeholders::_1));
#define REGISTER_MOUSE_LISTENER(eventType, func, arg) events::EventHandler::m_GetDispatchInstance()->m_mouseDispatch.RegisterListener(eventType, std::bind(&func, arg, std::placeholders::_1));
#define REGISTER_KEY_LISTENER(eventType, func, arg) events::EventHandler::m_GetDispatchInstance()->m_keyDispatch.RegisterListener(eventType, std::bind(&func, arg, std::placeholders::_1));
#define DISPATCH_BUTTON_EVENT(givenEvent) events::EventHandler::m_GetDispatchInstance()->m_buttonDispatch.DispatchToListeners(givenEvent)
#define DISPATCH_MOUSE_EVENT(givenEvent) events::EventHandler::m_GetDispatchInstance()->m_mouseDispatch.DispatchToListeners(givenEvent)
#define DISPATCH_KEY_EVENT(givenEvent) events::EventHandler::m_GetDispatchInstance()->m_keyDispatch.DispatchToListeners(givenEvent)
}