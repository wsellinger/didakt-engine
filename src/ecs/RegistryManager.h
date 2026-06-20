#pragma once

#include <entt/entity/registry.hpp>
#include <entt/entity/entity.hpp>
#include "entt/entity/fwd.hpp"

class RegistryManager
{
public:
    RegistryManager() = default;
    ~RegistryManager() = default;

    RegistryManager(const RegistryManager&) = delete;
    RegistryManager& operator=(const RegistryManager&) = delete;
    RegistryManager(RegistryManager&&) = delete;
    RegistryManager& operator=(RegistryManager&&) = delete;

    entt::entity CreateEntity();
    void DestroyEntity(entt::entity entity);
    entt::registry& GetRegistry() { return _registry; }

private:
    entt::registry _registry{};
};