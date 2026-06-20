#include "MovementSystem.h"

#include <entt/entity/registry.hpp>
#include <entt/entity/fwd.hpp>

#include "../components/TransformComponent.h"
#include "../components/RigidBodyComponent.h"

void MovementSystem::Update(entt::registry& registry, double deltaTime)
{
	//Get View
    auto view = registry.view<TransformComponent, RigidBodyComponent>();

	for (auto entity : view)
	{
		//Get Components
		auto& transform = view.get<TransformComponent>(entity);
		const auto& rigidBody = view.get<RigidBodyComponent>(entity);

		//Process Logic
		transform.position += rigidBody.velocity * static_cast<float>(deltaTime);
	}
}