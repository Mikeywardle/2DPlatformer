/*
* Custom Events classes created for implementation of Observer pattern
*
* Class is able to be invoked with any combination of arguments
* and store any reference to any function in any class with the correct arguments
*/
#pragma once

#include <vector>

// Base class of Delegate that contains no type reference
// to the class that is subscribed to the event
template<typename ... Args>
class EventDelegate
{
public:
	virtual void Execute(Args... args) {}
};

// Actual Delegate class that stores a reference to the
// Function to be called
template<class Clazz, typename ... Args>
class Delegate : public EventDelegate<Args...> {

	// Defining type Function for ease of reading
	typedef void(Clazz::* Function)(Args...);

public:
	Delegate() {}

	///<summary>
	/// Main constructor used to store refences to
	///</summary>
	Delegate(Clazz* object, Function function)
	{
		this->object = object;
		this->function = function;
	}

	///<summary>
	///
	///</summary
	virtual void Execute(Args... args) override
	{
		(object->*function)(args...);
	}

	Clazz* object;
	Function function;
};

// The main Event class. Can have listeners of void methods with parameter type(s) Args...
template <typename ... Args>
class Event {
public:

	///<summary>
	/// Calls all funcions that are currently listening to the event
	///</summary>
	void Invoke(Args... args) {
		for (EventDelegate<Args...>* delegate : delegates) {
			delegate->Execute(args...);
		}
	}

	///<summary>
	/// Create and add an EventDelegate with reference to method that will be executed on invoke()
	/// <para> EventDelegate is returned for use when removing listener
	///</summary>
	template<typename Clazz, typename Function>
	EventDelegate<Args...>* AddListener(Clazz* object, Function function)
	{
		typedef void(*InFunction)(void*, Args...);

		Delegate<Clazz, Args...>* delegate = new Delegate<Clazz, Args...>(object, function);
		delegates.push_back(delegate);

		return (EventDelegate<Args...>*)delegate;
	}

	///<summary>
	///Removes argument delegate from event. NOTE: Doing so will make the argument delegate null
	///</summary>
	void RemoveListener(EventDelegate<Args...>* e)
	{
		EventDelegate<Args...>* currentDelegate;

		for (int i = 0; i < delegates.size(); i++) {
			currentDelegate = delegates[i];

			if (currentDelegate == e)
			{
				delegates.erase(delegates.begin() + i);
				delete currentDelegate;
				return;
			}
		}


	}

	///<summary>
	///Removes all currently stored delegates from event. 
	///<para> NOTE: Doing so will make ALL delegates listening to this event null;
	///</summary>
	void RemoveAllListeners()
	{
		for (EventDelegate<Args...>* delegate : delegates) {
			delete delegate;
		}
		delegates.clear();
	}

private:
	//A collection of delegates to be executed on Invoke()
	std::vector<EventDelegate<Args...>*> delegates;
};