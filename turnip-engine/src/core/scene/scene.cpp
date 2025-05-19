#include "pch.hpp"
#include "scene.hpp"
#include "entity.hpp"
#include "components.hpp"
#include "core/script/script_system.hpp"

namespace tur
{
	void Scene::start_runtime()
	{
		ScriptSystem::on_scene_runtime_start(this);
		m_RuntimePlaying = true;
	}
	void Scene::on_update_runtime()
	{
		if (!m_RuntimePlaying)
			return;

		ScriptSystem::on_scene_runtime_update();
	}
	void Scene::stop_runtime()
	{
		m_RuntimePlaying = false;
	}

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

		// Script:
		entity.add_component<EntityScriptsComponent>();

		// Record:
		++m_Diagnostics.entityCount;

		return entity;
	}

	void Scene::remove_entity(Entity entity)
	{
		m_Registry.destroy(entity);
	}
}
