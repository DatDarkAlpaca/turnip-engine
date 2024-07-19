#pragma once
#include <entt/entt.hpp>
#include <optional>

#include "Util/UUID/UUID.hpp"

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

	public:
		uint32_t EntityCount() const { return m_Diagnostics.entityCount; }

	private:
		entt::registry m_Registry;
		std::unordered_map<UUID, Entity> m_EntityMap;

		struct Diagnostics
		{
			uint32_t entityCount = 0;
		} m_Diagnostics;
	};
}