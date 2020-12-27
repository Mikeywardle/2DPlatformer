#pragma once
#include <vector>
#include "Behaviour.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void OnStart();
	virtual void OnDestroy();

	class Level* GetLevel() const;
	void SetLevel(class Level* level);

	void SetActive(bool isActive);
	bool GetActive() const;

//Adding/Removing Components/Behaviours
public:
	template<class T>
	T* AddBehaviour()
	{
		T* toReturn = new T(this);

		Behaviour* newBehaviour = dynamic_cast<Behaviour*>(toReturn);
		behaviours.push_back(newBehaviour);

		return toReturn;
	}

protected:
	virtual void OnFrame(float deltaTime) {};

	bool canBeActive = true;
	bool m_IsActive = false;

	class Level* currentLevel;

private:
	std::vector<Behaviour*> behaviours;

	class EventDelegate<float>* frameDelegate;
};
