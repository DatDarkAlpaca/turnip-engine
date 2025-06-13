#include "pch.hpp"
#include "core/scene/components.hpp"

#include "renderer_assembler_system.hpp"

namespace tur
{
	// Texture Asset:
	static void batch_texture(RendererAssemblerSystem& system, asset_handle assetHandle)
	{
		const TextureAsset& textureAsset = system.assetLibrary->textures.get(assetHandle);

		// TODO: use a TextureFormat based on the TextureDataFormat provided.
		// TODO: provide additional texture options (needs an AssetLibrary viewer)

		TextureDescriptor textureDescriptor;
		{
			textureDescriptor.format = TextureFormat::RGBA8_UNORM;
			textureDescriptor.type = TextureType::TEXTURE_2D;
			textureDescriptor.width = textureAsset.width;
			textureDescriptor.height = textureAsset.height;
			textureDescriptor.generateMipmaps = true;
		}

		system.textureMap[textureAsset.uuid] = system.graphicsDevice->create_texture(textureDescriptor, textureAsset);
	}

	// Quad Renderer:
	static void assign_entity_texture_uuid(RendererAssemblerSystem& system, entt::entity entity, const UUID& uuid)
	{
		system.scene->get_registry().get<QuadTexture2D>(entity).textureUUID = uuid;
	}

	static void assign_entity_textures(RendererAssemblerSystem& system)
	{
#ifdef TUR_DEBUG
		if (!system.scene)
			return TUR_LOG_WARN("Attempted to assign textures without setting a scene first");
#endif

		for (auto& [entity, textureComponent] : system.scene->get_registry().view<QuadTexture2D>().each())
		{
			if (system.textureMap.find(textureComponent.textureUUID) == system.textureMap.end())
				continue;
			
			textureComponent.textureHandle = system.textureMap.at(textureComponent.textureUUID);
		}
	}

	static void remove_entity_texture(RendererAssemblerSystem& system, entt::entity entity)
	{
		auto& texture = system.scene->get_registry().get<QuadTexture2D>(entity);

		texture.descriptorHandle = invalid_handle;
		texture.textureHandle = invalid_handle;
		texture.textureUUID = invalid_uuid;
	}
}

namespace tur
{
	void initialize_renderer_assembler_system(RendererAssemblerSystem& system, NON_OWNING GraphicsDevice* graphicsDevice, NON_OWNING AssetLibrary* assetLibrary, NON_OWNING QuadRenderer* quadRenderer)
	{
		system.graphicsDevice = graphicsDevice;
		system.assetLibrary = assetLibrary;
		system.quadRenderer = quadRenderer;
	}
	void set_renderer_assembler_system_scene(RendererAssemblerSystem& system, NON_OWNING Scene* scene)
	{
		system.scene = scene;
	}

	void on_event_renderer_assembler(RendererAssemblerSystem& system, Event& event)
	{
		Subscriber subscriber(event);
		subscriber.subscribe<OnNewTextureLoad>([&](const OnNewTextureLoad& onLoadEvent) -> bool {
			batch_texture(system, onLoadEvent.assetHandle);

			const TextureAsset& textureAsset = system.assetLibrary->textures.get(onLoadEvent.assetHandle);
			assign_entity_textures(system);
			return false;
		});

		subscriber.subscribe<SceneQuadTextureLoaded>([&](const SceneQuadTextureLoaded& sceneQuadLoadEvent) -> bool {
			assign_entity_texture_uuid(system, sceneQuadLoadEvent.entity, sceneQuadLoadEvent.textureUUID);
			return false;
		});

		subscriber.subscribe<SceneQuadTextureUnloaded>([&](const SceneQuadTextureUnloaded& sceneQuadUnloadEvent) -> bool {
			remove_entity_texture(system, sceneQuadUnloadEvent.entity);
			return false;
		});
	}
}
