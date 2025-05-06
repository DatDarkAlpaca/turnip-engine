#pragma once
#include <string>
#include "graphics/graphics.hpp"
#include "utils/uuid/uuid.hpp"
#include "utils/transform.hpp"

namespace tur
{
	struct UUIDComponent
	{
	public:
		UUIDComponent() = default;
		UUIDComponent(UUID uuid) : uuid(uuid) { }
		UUIDComponent(const UUIDComponent&) = default;

	public:
		UUID uuid;
	};

	struct NameComponent
	{
	public:
		NameComponent() = default;
		NameComponent(const NameComponent&) = default;
		NameComponent(const std::string& name) : name(name) { }

	public:
		std::string name;
	};

	struct SceneGraphComponent
	{
	public:
		SceneGraphComponent() = default;
		SceneGraphComponent(const SceneGraphComponent&) = default;

	public:
		entt::entity parent = entt::null;
		std::vector<entt::entity> children;
	};

	struct TransformComponent 
	{
	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale)
			: transform(position, rotation, scale) 
		{
		}

	public:
		operator Transform()
		{
			return transform;
		}
		operator glm::mat4()
		{
			return transform.transform();
		}

	public:
		Transform transform;
	};

	struct TextureComponent
	{
	public:
		TextureComponent() = default;
		TextureComponent(const TextureComponent&) = default;
		TextureComponent(texture_handle handle, const std::filesystem::path& filepath) 
			: handle(handle) 
			, filepath(filepath)
		{ }

	public:
		std::filesystem::path filepath;
		texture_handle handle = 0;
	};
}