#pragma once
#include <entt/entt.hpp>
#include <optional>

#include "utils/uuid/uuid.hpp"

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
		Entity add_entity(UUID uuid, const std::string& entityName, entt::entity parent = entt::null);

		void remove_entity(UUID uuid);
		void remove_entity(Entity entity);

		std::optional<Entity> find_entity_by_uuid(UUID uuid) const;
		std::vector<Entity> find_entities_by_name(const std::string& name);

	public:
		inline u64 get_entity_count() const { return m_Diagnostics.entityCount; }

		inline entt::registry& get_registry() { return m_Registry; }

	private:
		entt::registry m_Registry;
		std::unordered_map<UUID, Entity> m_EntityMap;

		struct Diagnostics
		{
			u64 entityCount = 0;
		} m_Diagnostics;
	};
}