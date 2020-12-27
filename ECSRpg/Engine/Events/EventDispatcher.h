#pragma once
#include "Event.h"

class IEventDispatcher
{
};

template<typename T>
class EventDispatcher : public IEventDispatcher
{
public:
	template<typename Clazz, typename Function>
	EventDelegate<T>* AddListener(Clazz* object, Function function)
	{
		return event.AddListener(object, function);
	}

	void RemoveListener(EventDelegate<T>* delegate)
	{
		event.RemoveListener(delegate);
	}

	void DispatchMessage(T message)
	{
		event.Invoke(message);
	}

private:
	Event<T> event;
};
