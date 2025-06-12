#pragma once
#include <unordered_map>

#include "graphics/graphics_device.hpp"
#include "core/assets/asset_library.hpp"
#include "quad/quad_renderer.hpp"
#include "core/scene/scene.hpp"
#include "utils/uuid/uuid.hpp"

namespace tur
{
	// TODO: add instanced

	struct RendererAssemblerSystem
	{
		NON_OWNING GraphicsDevice* graphicsDevice;
		NON_OWNING AssetLibrary* assetLibrary;
		NON_OWNING QuadRenderer* quadRenderer;
		NON_OWNING Scene* scene;

		std::unordered_map<UUID, texture_handle> textureMap;
	};

	void initialize_renderer_assembler_system(RendererAssemblerSystem& system, NON_OWNING GraphicsDevice* graphicsDevice, NON_OWNING AssetLibrary* assetLibrary, NON_OWNING QuadRenderer* quadRenderer);
	void set_renderer_assembler_system_scene(RendererAssemblerSystem& system, NON_OWNING Scene* scene);

	void on_event_renderer_assembler(RendererAssemblerSystem& system, Event& event);
}