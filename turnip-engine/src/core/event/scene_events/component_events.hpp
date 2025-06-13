#pragma once
#include <entt/entt.hpp>
#include "core/event/event.hpp"
#include "graphics/graphics.hpp"

namespace tur
{
	class SceneQuadTextureLoaded : public Event
	{
		DEFINE_EVENT(EventType::SCENE_QUAD_TEXTURE_LOADED);

	public:
		SceneQuadTextureLoaded(entt::entity entity, const UUID& textureUUID)
			: entity(entity)
			, textureUUID(textureUUID)
		{

		}

	public:
		entt::entity entity = entt::null;
		UUID textureUUID = invalid_uuid;
	};

	class SceneQuadTextureUnloaded : public Event
	{
		DEFINE_EVENT(EventType::SCENE_QUAD_TEXTURE_UNLOADED);

	public:
		SceneQuadTextureUnloaded(entt::entity entity)
			: entity(entity)
		{

		}

	public:
		entt::entity entity = entt::null;
	};
}