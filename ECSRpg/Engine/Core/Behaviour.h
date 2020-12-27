#pragma once

#include <Events/Event.h>

class Behaviour
{
public:
	Behaviour(class GameObject* parent);

	virtual void OnStart();
	virtual void OnDestroy();

	void SetActive(bool isActive);
	bool GetActive() const;

protected:

	virtual void OnFrame(float deltaTime) = 0;

	bool m_IsActive = false;

private:
	class GameObject* parent;

	class EventDelegate<float>* frameDelegate;
};
