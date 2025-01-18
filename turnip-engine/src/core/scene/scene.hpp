#pragma once
#include <entt/entt.hpp>
#include <optional>

#include "Utils/UUID/UUID.hpp"

namespace tur
{
	class Entity;

	class Scene
	{
		friend class Entity;

	public:
		Scene() = default;

	public:
		Entity add_entity();
		Entity add_entity(const std::string& entityName);
		Entity add_entity(UUID uuid, const std::string& entityName);

		void remove_entity(UUID uuid);
		void remove_entity(Entity entity);

		std::optional<Entity> find_entity_by_uuid(UUID uuid) const;
		std::vector<Entity> find_entities_by_name(const std::string& name);

	public:
		inline uint64_t get_entity_count() const { return m_Diagnostics.entityCount; }

	private:
		entt::registry m_Registry;
		std::unordered_map<UUID, Entity> m_EntityMap;

		struct Diagnostics
		{
			uint64_t entityCount = 0;
		} m_Diagnostics;
	};
}