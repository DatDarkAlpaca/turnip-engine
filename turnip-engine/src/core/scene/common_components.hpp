#pragma once
#include <string>
#include "graphics/graphics.hpp"
#include "utils/uuid/uuid.hpp"

namespace tur
{
	struct UUIDComponent
	{
	public:
		UUIDComponent() = default;
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

	struct TransformComponent 
	{
	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) : transform(transform) { }

	public:
		glm::mat4 transform;
	};

	struct TextureComponent
	{
	public:
		TextureComponent() = default;
		TextureComponent(const TextureComponent&) = default;
		TextureComponent(texture_handle handle) : handle(handle) { }

	public:
		texture_handle handle;
	};
}