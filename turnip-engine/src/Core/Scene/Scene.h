#pragma once
#include "pch.h"
#include "Entity.h"

namespace tur
{
	class Scene
	{
	public:
		Scene()
		{
			m_Registry = std::make_shared<entt::registry>();
		}

	public:
		Entity NewEntity()
		{
			return Entity(m_Registry->create(), m_Registry);
		}

		void DeleteEntity(entt::entity entity)
		{
			m_Registry->destroy(entity);
		}

	public:
		const std::shared_ptr<entt::registry>& GetRegistry() const { return m_Registry; }

	private:
		std::shared_ptr<entt::registry> m_Registry;
	};
}