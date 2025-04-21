#include "pch.hpp"
#include "scene.hpp"
#include "entity.hpp"
#include "common_components.hpp"

namespace tur
{
	Entity Scene::add_entity()
	{
		return add_entity({ }, { });
	}

	Entity Scene::add_entity(const std::string& entityName)
	{
		return add_entity({ }, entityName);
	}

	Entity Scene::add_entity(UUID uuid, const std::string& entityName, entt::entity parent)
	{
		Entity entity = { m_Registry.create(), this };
		
		// UUID:
		UUIDComponent& uuidComp = entity.add_component<UUIDComponent>(uuid);
	
		// Name:
		NameComponent& nameComp = entity.add_component<NameComponent>();
		{
			nameComp.name = entityName.empty()
				? "Entity #" + std::to_string(m_Diagnostics.entityCount) 
				: entityName;
		}

		// Scene Graph:
		SceneGraphComponent& sceneGraphComp = entity.add_component<SceneGraphComponent>();
		sceneGraphComp.parent = parent;

		// Record:
		m_EntityMap[uuidComp.uuid] = entity;
		++m_Diagnostics.entityCount;

		return entity;
	}

	void Scene::remove_entity(UUID uuid)
	{
		m_EntityMap.erase(uuid);

		auto& entityOption = find_entity_by_uuid(uuid);
		if (!entityOption.has_value())
		{
			TUR_LOG_WARN("Failed to remove entity with UUID: {}", static_cast<u64>(uuid));
			return;
		}

		m_Registry.destroy(entityOption.value());
	}

	void Scene::remove_entity(Entity entity)
	{
		m_EntityMap.erase(entity.UUID());
		m_Registry.destroy(entity);
	}

	std::optional<Entity> Scene::find_entity_by_uuid(UUID uuid) const
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return m_EntityMap.at(uuid);

		return std::nullopt;
	}

	std::vector<Entity> Scene::find_entities_by_name(const std::string& name)
	{
		std::vector<Entity> entities;
		for (auto [entity, nameComp]: m_Registry.view<NameComponent>().each())
		{
			if (nameComp.name == name)
				entities.push_back(Entity(entity, this));
		}

		return entities;
	}
}
