#pragma once
#include "IEvent.h"

namespace tur
{
	class Subscriber
	{
		template<typename ConcreteEvent>
		using SubscrictionCallback = std::function<bool(ConcreteEvent&)>;

	public:
		Subscriber(IEvent& event)
			: m_Event(event)
		{

		}

	public:
		template<typename ConcreteEvent>
		void SubscribeTo(SubscrictionCallback<ConcreteEvent> callback)
		{
			if (m_Event.m_Handled)
				return;

			if (ConcreteEvent::Type() == m_Event.GetType())
			{
				ConcreteEvent& concreteEvent = static_cast<ConcreteEvent&>(m_Event);
				m_Event.m_Handled = m_Event.m_Handled || callback(concreteEvent);
			}
		}

	private:
		IEvent& m_Event;
	};
}