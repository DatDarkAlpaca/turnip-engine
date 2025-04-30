#pragma once
#include <entt/entt.hpp>

#include "scene.hpp"
#include "common.hpp"
#include "common_components.hpp"

namespace tur
{
	class Entity
	{
	public:
		Entity() = default;

		Entity(entt::entity entityHandle, Scene* sceneHandle)
			: m_EntityHandle(entityHandle)
			, m_Scene(sceneHandle)
		{

		}

	public:
		template<typename T, typename... Args>
		T& add_component(Args&&... args)
		{
			// Todo: check whether the component accepts duplicates.

			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			return component;
		}

		template<typename T>
		void remove_component()
		{
			// Todo: check whether the component exists.
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		const T& get_component() const
		{
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		T& get_component()
		{
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T>
		bool has_component() const
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

	public:
		inline UUID UUID() const { return get_component<UUIDComponent>().uuid; }
		inline std::string Name() const { return get_component<NameComponent>().name; }

	public:
		operator entt::entity() const { return m_EntityHandle; }

		entt::entity get_handle() const { return m_EntityHandle; }

	private:
		entt::entity m_EntityHandle = entt::null;
		NON_OWNING Scene* m_Scene = nullptr;
	};
}