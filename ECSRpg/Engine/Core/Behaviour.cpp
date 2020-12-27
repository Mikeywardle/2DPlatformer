#include "Behaviour.h"
#include "GameObject.h"
#include <Events/Event.h>
#include "Level.h"

Behaviour::Behaviour(GameObject* parent)
{
	this->parent = parent;
}

void Behaviour::OnStart()
{
	if (m_IsActive)
		frameDelegate = parent->GetLevel()->AddFrameCallBack(this, &Behaviour::OnFrame);
}

void Behaviour::OnDestroy()
{
}

void Behaviour::SetActive(bool isActive)
{
	if (isActive && !m_IsActive)
		frameDelegate = parent->GetLevel()->AddFrameCallBack(this, &Behaviour::OnFrame);
	else if (!isActive && m_IsActive)
		parent->GetLevel()->RemoveFrameCallback(frameDelegate);

	m_IsActive = isActive;
}

bool Behaviour::GetActive() const
{
	return m_IsActive;
}
