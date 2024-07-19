#include "pch.hpp"
#include "Scene.hpp"
#include "Entity.hpp"
#include "CommonComponents.hpp"

namespace tur
{
	Entity Scene::AddEntity()
	{
		return AddEntity({ }, { });
	}

	Entity Scene::AddEntity(const std::string& entityName)
	{
		return AddEntity({ }, entityName);
	}

	Entity Scene::AddEntity(UUID uuid, const std::string& entityName)
	{
		Entity entity = { m_Registry.create(), this };
		
		// UUID:
		IdentifierComponent& uuidComp = entity.AddComponent<IdentifierComponent>(uuid);
	
		// Name:
		NameComponent& nameComp = entity.AddComponent<NameComponent>();
		{
			nameComp.name = entityName.empty() ? "Entity " + std::to_string(m_Diagnostics.entityCount) : entityName;
		}

		// Record:
		m_EntityMap[uuidComp.uuid] = entity;
		m_Diagnostics.entityCount++;

		return entity;
	}

	void Scene::RemoveEntity(UUID uuid)
	{
		m_EntityMap.erase(uuid);

		auto& entityOption = FindEntityByUUID(uuid);
		if (!entityOption.has_value())
		{
			TUR_LOG_WARN("Failed to remove entity with UUID: {}", std::uint64_t(uuid));
			return;
		}

		m_Registry.destroy(entityOption.value());
	}

	void Scene::RemoveEntity(Entity entity)
	{
		m_EntityMap.erase(entity.UUID());
		m_Registry.destroy(entity);
	}

	std::optional<Entity> Scene::FindEntityByUUID(UUID uuid) const
	{
		if (m_EntityMap.find(uuid) != m_EntityMap.end())
			return m_EntityMap.at(uuid);

		return std::nullopt;
	}

	std::vector<Entity> Scene::FindEntitiesByName(const std::string& name)
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
