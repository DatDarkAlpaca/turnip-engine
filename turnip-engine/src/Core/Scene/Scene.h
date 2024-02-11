#pragma once
#include <entt/entt.hpp>
#include <optional>

#include "Util/UUID/UUID.h"

namespace tur
{
	class Entity;

	class Scene
	{
		friend class Entity;

	public:
		Scene() = default;

	public:
		Entity AddEntity();
		Entity AddEntity(const std::string& entityName);
		Entity AddEntity(UUID uuid, const std::string& entityName);

		void RemoveEntity(UUID uuid);
		void RemoveEntity(Entity entity);

		std::optional<Entity> FindEntityByUUID(UUID uuid) const;
		std::vector<Entity> FindEntitiesByName(const std::string& name);

	private:
		entt::registry m_Registry;
		std::unordered_map<UUID, Entity> m_EntityMap;

		uint32_t m_EntityCount = 0;
	};
}