#pragma once
#include "event.hpp"

namespace tur
{
	class Subscriber
	{
	public:
		Subscriber(Event& event)
			: m_Event(event)
		{

		}

	public:
		template<typename ImplementedEvent>
		void subscribe(std::function<bool(ImplementedEvent)> callback)
		{
			if (m_Event.handled)
				return;

			if (m_Event.id() != ImplementedEvent::get_id())
				return;

			ImplementedEvent* implementedEvent = static_cast<ImplementedEvent*>(&m_Event);
			implementedEvent->handled = implementedEvent->handled || callback(*implementedEvent);
		}

	private:
		Event& m_Event;
	};
}