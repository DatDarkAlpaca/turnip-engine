#pragma once
#include <glm/glm.hpp>
#include "core/scene/scene.hpp"
#include "velvet_components.hpp"

namespace tur
{
	// 1. Kinematics System
	// * Velvet integrator
	// * Accumulated timestep for physics

	// 2. Broad collision detection:
	// * 2D space partition through spatial hashing

	// 3. Narrow collision detection:
	// * SAT

	inline void velvet_integrate(Rigidbody2DComponent& rb, const glm::vec2& force, float fdt)
	{
		glm::vec2 tempPosition = rb.position;

		rb.position = 2.0f * rb.position - rb.oldPosition + (force * fdt * fdt / rb.mass);
		rb.oldPosition = tempPosition;
	}

	inline void velvet_system_2d_initialize(Scene& scene)
	{
		auto& registry = scene.get_registry();

		for (auto& [entity, rb2d] : registry.view<Rigidbody2DComponent>(entt::exclude<TransformComponent>).each())
		{
			TUR_LOG_WARN("Rigidbody objects without a transform component detected. A default transform has been added.");
			registry.emplace<TransformComponent>(entity);
		}

		for (auto& [_, transform, rb2d] : registry.view<TransformComponent, Rigidbody2DComponent>().each())
		{
			rb2d.position = transform.transform.position;
		}
	}

	inline void velvet_system_2d_add_force(Rigidbody2DComponent& rb, const glm::vec2& force)
	{

	}

	inline void velvet_system_2d_update(Scene& scene, float fdt)
	{
		// Integration:
		for (auto& [_, transform, rb2d] : scene.get_registry().view<TransformComponent, Rigidbody2DComponent>().each())
		{
			velvet_integrate(rb2d, {}, fdt);
			rb2d.position = transform.transform.position;
		}

		// Broad phase

		// Narrow phase
	}
}