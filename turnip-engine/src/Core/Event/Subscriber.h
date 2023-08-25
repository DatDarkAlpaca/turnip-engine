#pragma once
#include "Event.h"

namespace tur
{
	class Subscriber
	{
		template<typename ConcreteEventData>
		using SubscrictionCallback = std::function<bool(ConcreteEventData*)>;

	public:
		Subscriber(Event& event)
			: event(event) { }

	public:
		template<typename ConcreteEventData>
		void SubscribeTo(SubscrictionCallback<ConcreteEventData>&& callback)
		{
			if (event.handled)
				return;

			if (ConcreteEventData::type == event.type)
			{
				ConcreteEventData* eventData = static_cast<ConcreteEventData*>(event.data.get());
				event.handled = event.handled || callback(eventData);
			}
		}

	private:
		Event& event;
	};
}