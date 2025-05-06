#pragma once
#include <vector>
#include <entt/entt.hpp>
#include "common.hpp"

namespace tur
{
	class SceneInstanceHolder
	{
	public:
		void initialize(entt::registry* registry)
		{
			r_Registry = registry;
		}

	public:
		inline void add_instance(entt::entity entityID)
		{

		}

	private:
		NON_OWNING entt::registry* r_Registry = nullptr;
	};
}

