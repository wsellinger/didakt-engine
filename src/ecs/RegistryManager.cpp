#include "RegistryManager.h"

#include <entt/entity/entity.hpp>
#include <entt/entity/fwd.hpp>

entt::entity RegistryManager::CreateEntity()
{
    return _registry.create();
}

void RegistryManager::DestroyEntity(entt::entity entity)
{
    _registry.destroy(entity);
}
