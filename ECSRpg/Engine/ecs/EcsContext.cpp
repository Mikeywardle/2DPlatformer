#include "EcsContext.h"

ECSContext::ECSContext()
{
    entitiesManager = new EntitiesManager();
}

Entity ECSContext::CreateEntity()
{
    return entitiesManager->CreateEntity();
}

void ECSContext::DestroyEntity(Entity entity)
{
    entitiesManager->DestroyEntity(entity);

    std::map<const char*, IComponentBatch*>::iterator it = componentsLookup.begin();

    while (it != componentsLookup.end())
    {
        it->second->RemoveComponent(entity);
        ++it;
    }
}
