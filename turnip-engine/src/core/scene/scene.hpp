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
		void start_runtime();
		void on_update_runtime();
		void stop_runtime();

	public:
		Entity add_entity();
		Entity add_entity(const std::string& entityName);
		Entity add_entity(UUID uuid, const std::string& entityName, entt::entity parent = entt::null);

		void remove_entity(Entity entity);

	public:
		inline u64 get_entity_count() const { return m_Diagnostics.entityCount; }
		inline entt::registry& get_registry() { return m_Registry; }

	private:
		entt::registry m_Registry;
	
		struct Diagnostics
		{
			u64 entityCount = 0;
		} m_Diagnostics;

	private:
		bool m_RuntimePlaying = false;
	};
}