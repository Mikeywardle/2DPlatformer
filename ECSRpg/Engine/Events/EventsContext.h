#pragma once

#include <unordered_map>
#include "EventDispatcher.h"

class EventsContext
{

public:
	template<typename T, typename Clazz, typename Function>
	EventDelegate<T>* AddListener(Clazz* object, Function function)
	{
		return GetEventDispatcher<T>()->AddListener(object, function);
	}

	template<typename T>
	void RemoveListener(EventDelegate<T>* delegate)
	{
		GetEventDispatcher<T>()->RemoveListener(delegate);
	}

	template<typename T>
	void DispatchMessage(T message)
	{
		GetEventDispatcher<T>()->DispatchMessage(message);
	}

private:

	template<typename T>
	EventDispatcher<T>* GetEventDispatcher()
	{
		const char* hash = typeid(T).raw_name();

		if (EventDispatchers.find(hash) == EventDispatchers.end())
			EventDispatchers[hash] = (IEventDispatcher*) new EventDispatcher<T>();

		return (EventDispatcher<T>*)EventDispatchers[hash];
	};

	std::unordered_map<const char*, IEventDispatcher*> EventDispatchers;
};
