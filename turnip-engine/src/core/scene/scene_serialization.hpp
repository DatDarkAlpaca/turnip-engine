#pragma once
#include "scene.hpp"
#include "components.hpp"

#include "utils/json/json_writer.hpp"
#include "utils/json/json_reader.hpp"

namespace tur
{
	inline void serialize_entity(const std::filesystem::path& filepath, NON_OWNING Scene* scene, entt::entity entityID)
	{
		auto& registry = scene->get_registry();

		nlohmann::json json;
		std::string eid = std::to_string(static_cast<u32>(entityID));

		json["uuid"] = static_cast<u32>(registry.get<UUIDComponent>(entityID).uuid);
		json["name"] = registry.get<NameComponent>(entityID).name;

		// Scene Graph:
		const auto& sceneGraph = registry.get<SceneGraphComponent>(entityID);		
		json["sceneGraph"]["parent"] = static_cast<u32>(sceneGraph.parent);
		json["sceneGraph"]["children"] = sceneGraph.children;

		// Transform:
		const auto& tranformComponent = registry.get<TransformComponent>(entityID);
		const auto& transform = tranformComponent.transform;

		json["transform"]["position"]["x"] = transform.position.x;
		json["transform"]["position"]["y"] = transform.position.y;
		json["transform"]["position"]["z"] = transform.position.z;

		json["transform"]["rotation"]["x"] = transform.rotation.x;
		json["transform"]["rotation"]["y"] = transform.rotation.y;
		json["transform"]["rotation"]["z"] = transform.rotation.z;

		json["transform"]["scale"]["x"] = transform.scale.x;
		json["transform"]["scale"]["y"] = transform.scale.y;
		json["transform"]["scale"]["z"] = transform.scale.z;

		// Texture:
		const auto textureComponent = registry.get<TextureComponent>(entityID);
		json["texture"]["filepath"] = textureComponent.filepath.string();

		// Scripts:
		const auto& scriptComponents = registry.get<EntityScriptsComponent>(entityID);
		json["scripts"] = {};

		for (const auto& script : scriptComponents.scriptComponents)
		{
			nlohmann::json scriptJson;
			scriptJson["className"] = script.className;
			scriptJson["filepath"] = script.filepath;

			json["scripts"].push_back(scriptJson);
		}

		JsonWriter writer(filepath);
		writer.write(json);
	}

	class SceneSerializer
	{
	public:
		explicit SceneSerializer(NON_OWNING Scene* scene, const std::filesystem::path& filepath)
			: r_Scene(scene)
			, m_Writer(filepath)
		{
		}

	public:
		void serialize()
		{
			auto& registry = r_Scene->get_registry();

			nlohmann::json json;
			
			auto eid = [&](entt::entity entity) { return std::to_string(static_cast<u32>(entity)); };

			for (const auto& [entity, uuid] : registry.view<UUIDComponent>().each())
			{
				json[eid(entity)]["uuid"] = static_cast<u32>(uuid.uuid);
			}

			for (const auto& [entity, nameComponent] : registry.view<NameComponent>().each())
			{
				json[eid(entity)]["name"] = nameComponent.name;
			}

			for (const auto& [entity, sceneGraph] : registry.view<SceneGraphComponent>().each())
			{
				json[eid(entity)]["sceneGraph"]["parent"] = static_cast<u32>(sceneGraph.parent);
				json[eid(entity)]["sceneGraph"]["children"] = sceneGraph.children;
			}
		
			for (const auto& [entity, tranformComponent] : registry.view<TransformComponent>().each())
			{
				const auto& transform = tranformComponent.transform;

				json[eid(entity)]["transform"]["position"]["x"] = transform.position.x;
				json[eid(entity)]["transform"]["position"]["y"] = transform.position.y;
				json[eid(entity)]["transform"]["position"]["z"] = transform.position.z;

				json[eid(entity)]["transform"]["rotation"]["x"] = transform.rotation.x;
				json[eid(entity)]["transform"]["rotation"]["y"] = transform.rotation.y;
				json[eid(entity)]["transform"]["rotation"]["z"] = transform.rotation.z;

				json[eid(entity)]["transform"]["scale"]["x"]    = transform.scale.x;
				json[eid(entity)]["transform"]["scale"]["y"]    = transform.scale.y;
				json[eid(entity)]["transform"]["scale"]["z"]    = transform.scale.z;
			}

			for (const auto& [entity, texture] : registry.view<TextureComponent>().each())
			{
				json[eid(entity)]["texture"]["filepath"] = texture.filepath.string();
			}

			for (const auto& [entity, scripts] : registry.view<EntityScriptsComponent>().each())
			{
				json[eid(entity)]["scripts"] = {};

				for (const auto& script : scripts.scriptComponents)
				{
					nlohmann::json scriptJson;
					scriptJson["className"] = script.className;
					scriptJson["filepath"] = script.filepath;

					json[eid(entity)]["scripts"].push_back(scriptJson);
				}
			}

			m_Writer.write(json);
		}

	private:
		NON_OWNING Scene* r_Scene = nullptr;
		JsonWriter m_Writer;
	};

	class SceneDeserializer
	{
	public:
		explicit SceneDeserializer(NON_OWNING Scene* scene, const std::filesystem::path& filepath)
			: r_Scene(scene)
			, m_Reader(filepath)
		{
		}

	public:
		void deserialize()
		{
			auto& registry = r_Scene->get_registry();

			nlohmann::json json = m_Reader.parse();

			for (const auto& [entityKey, entityObj] : json.items())
			{
				u32 eid = static_cast<u32>(std::stod(entityKey));
				entt::entity entity = registry.create(entt::entity(eid));

				if (entityObj.contains("uuid"))
				{
					UUIDComponent uuidComponent({ static_cast<u64>(entityObj["uuid"])});
					registry.emplace<UUIDComponent>(entity, uuidComponent);
				}

				if (entityObj.contains("name"))
				{
					NameComponent nameComponent(entityObj["name"]);
					registry.emplace<NameComponent>(entity, nameComponent);
				}

				if (entityObj.contains("sceneGraph"))
				{
					SceneGraphComponent sceneGraphComponent;
					sceneGraphComponent.parent = entityObj["sceneGraph"]["parent"];

					if (entityObj["sceneGraph"].contains("children"))
					{
						for (const auto& child : entityObj["sceneGraph"]["children"])
							sceneGraphComponent.children.push_back(entt::entity(child));
					}

					registry.emplace<SceneGraphComponent>(entity, sceneGraphComponent);
				}

				if (entityObj.contains("transform"))
				{
					TransformComponent transformComponent;
					auto& transform = transformComponent.transform;

					{
						transform.position.x = entityObj["transform"]["position"]["x"];
						transform.position.y = entityObj["transform"]["position"]["y"];
						transform.position.z = entityObj["transform"]["position"]["z"];

						transform.rotation.x = entityObj["transform"]["rotation"]["x"];
						transform.rotation.y = entityObj["transform"]["rotation"]["y"];
						transform.rotation.z = entityObj["transform"]["rotation"]["z"];

						transform.scale.x    = entityObj["transform"]["scale"]["x"];
						transform.scale.y    = entityObj["transform"]["scale"]["y"];
						transform.scale.z    = entityObj["transform"]["scale"]["z"];
					}
					
					registry.emplace<TransformComponent>(entity, transformComponent);
				}
				
				if (entityObj.contains("texture"))
				{
					TextureComponent textureComponent;
					{
						textureComponent.filepath = std::string(entityObj["texture"]["filepath"]);					
					}

					registry.emplace<TextureComponent>(entity, textureComponent);
				}

				if (entityObj.contains("scripts"))
				{
					EntityScriptsComponent scriptsComponent;
					
					if (!entityObj["scripts"].empty())
					{
						for (const auto& script : entityObj["scripts"])
						{
							InternalEntityScript internalScript;
							internalScript.className = script["className"];
							internalScript.filepath = std::string(script["filepath"]);
							scriptsComponent.scriptComponents.push_back(internalScript);
						}
					}

					registry.emplace<EntityScriptsComponent>(entity, scriptsComponent);
				}
			}
		}

	private:
		NON_OWNING Scene* r_Scene = nullptr;
		JsonReader m_Reader;
	};
}