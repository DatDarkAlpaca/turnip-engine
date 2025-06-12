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

	struct QuadTexture2D
	{
	public:
		QuadTexture2D() = default;
		QuadTexture2D(const QuadTexture2D&) = default;
		QuadTexture2D(UUID textureUUID, texture_handle textureHandle)
			: textureHandle(textureHandle)
			, textureUUID(textureUUID)
		{ }

	public:
		UUID textureUUID = invalid_uuid;
		texture_handle textureHandle = invalid_handle;
		descriptor_handle descriptorHandle = invalid_handle;
	};

	struct InstancedQuadComponent
	{
	public:
		InstancedQuadComponent() = default;
		InstancedQuadComponent(const InstancedQuadComponent&) = default;
		InstancedQuadComponent(u32 textureLayer)
			: textureLayer(textureLayer)
		{
		}

	public:
		u32 textureLayer = 0;

	public:
		bool dirtyFlag = true;
		u32 instanceIndex = invalid_handle;
	};
}