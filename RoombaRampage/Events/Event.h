#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <vector>
#include <unordered_map>

namespace Event{

	enum EventType {


	};

	class EventSystem {
		//TURN TO SINGLETON?

	public:
		using EventListener = std::function<void()>; // function pointing to the "reaction" of the event

		void SubscribeEvent(EventType, EventListener listener);

		//void UnsubscribeEvent(EventType, EventListener listener);

		void TriggerEvent(EventType);

	private:
		//				Type of Event		Vector of "subscribers"
		std::unordered_map<EventType, std::vector<EventListener>> Listeners;

	};







}




#endif EVENT_H
