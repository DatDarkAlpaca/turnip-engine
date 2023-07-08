#pragma once
#include "pch.h"

namespace tur
{
	class Entity
	{
	public:
		Entity(entt::entity entity, const std::shared_ptr<entt::registry>& registry)
			: m_Entity(entity), registry(registry)
		{

		}

	public:
		template<typename ComponentType, typename... Args>
		ComponentType& AddComponent(Args&&... args)
		{
			auto lockedRegistry = registry.lock();
			return lockedRegistry->emplace<ComponentType>(m_Entity, std::forward<decltype(args)>(args)...);
		}

	private:
		entt::entity m_Entity { entt::null };
		std::weak_ptr<entt::registry> registry;
	};
}