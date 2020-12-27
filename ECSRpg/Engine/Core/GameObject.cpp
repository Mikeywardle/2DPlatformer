#include "GameObject.h"
#include "Level.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::OnStart()
{
    if (canBeActive)
    {
        frameDelegate = currentLevel->AddFrameCallBack(this, &GameObject::OnFrame);
        m_IsActive = true;
    }

    for (Behaviour* behaviour : behaviours)
    {
        behaviour->OnStart();
    }
}

void GameObject::OnDestroy()
{
}

Level* GameObject::GetLevel() const
{
    return currentLevel;
}

void GameObject::SetLevel(Level* level)
{
    currentLevel = level;
}

void GameObject::SetActive(bool isActive)
{
    if (isActive && !m_IsActive)
        frameDelegate = currentLevel->AddFrameCallBack(this, &GameObject::OnFrame);
    else if (!isActive && m_IsActive)
        currentLevel->RemoveFrameCallback(frameDelegate);

    m_IsActive = isActive;
}

bool GameObject::GetActive() const
{
    return m_IsActive;
}
