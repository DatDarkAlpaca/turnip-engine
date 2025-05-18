#include "pch.hpp"
#include "main_view.hpp"
#include <core/scene/scene_serialization.hpp>

MainView::MainView(const ProjectData& projectData)
	: m_ProjectData(projectData)
{
	m_MainMenuBar.initialize(this);
}

void MainView::set_project_data(const ProjectData& projectData)
{
	m_ProjectData = projectData;

	// Reset:
	scene.get_registry().clear();
	m_SceneData.viewerSelectedEntity = entt::null;

	ScriptSystem::set_project(m_ProjectData);

	// Deserialize main scene:
	if (!std::filesystem::is_regular_file(projectData.projectPath / "scene.json"))
		return;

	SceneDeserializer deserializer(&scene, projectData.projectPath / "scene.json");
	deserializer.deserialize();

	// Load assets:
	for (const auto& [entity, textureComponent] : scene.get_registry().view<TextureComponent>().each())
	{
		auto* assetLibrary = &r_Engine->get_asset_library();
		auto* graphicsDevice = &r_Engine->get_graphics_device();

		r_Engine->get_worker_pool().submit<AssetInformation>([assetLibrary, textureComponent]() {
			return load_texture_asset(assetLibrary, textureComponent.filepath);
		}, [&textureComponent, assetLibrary, graphicsDevice](AssetInformation information) {
			// if (information.isDuplicate)
			//	return;
			// TODO: map asset handle to texture handle on create_texture.			[URGENT]
			// TODO: maybe save this threaded function since i use it everywhere

			auto texture = assetLibrary->textures.get(information.handle);

			TextureDescriptor descriptor;
			{
				descriptor.format = TextureFormat::RGBA8_UNORM;
				descriptor.type = TextureType::TEXTURE_2D;
				descriptor.width = texture.width;
				descriptor.height = texture.height;
				descriptor.generateMipmaps = true;
			}

			auto textureGraphics = graphicsDevice->create_texture(descriptor, texture);
			textureComponent.handle = textureGraphics;
		});
	}
}

void MainView::on_view_added()
{
	set_project_data(m_ProjectData);

	initialize_textures();
	initialize_renderer_system();

	// Widgets:
	m_EntityInspector.initialize(r_Engine, &scene, &m_SceneData);
	m_SceneViewer.initialize(&scene, &m_SceneData);
	m_SceneEditor.initialize(&r_Engine->get_graphics_device(), &r_Engine->get_window(), &m_SceneData);
}
void MainView::on_update()
{
	if (m_SceneData.projectEdited)
		append_window_title(m_ProjectData.projectName + "*");
	else
		append_window_title(m_ProjectData.projectName);

	scene.on_update_runtime();
}
void MainView::on_render_gui()
{
	ImGui::DockSpaceOverViewport();

	m_MainMenuBar.on_render();

	{
		ImGui::Begin("Runtime");

		if (ImGui::Button("Start"))
			scene.start_runtime();

		ImGui::SameLine();

		if (ImGui::Button("Stop"))
			scene.stop_runtime();

		ImGui::End();
	}

	m_SceneEditor.on_render_gui();
	m_SceneViewer.on_render_gui();
	m_EntityInspector.on_render_gui();
}
void MainView::on_event(Event& event)
{
	Subscriber subscriber(event);
	subscriber.subscribe<WindowResizeEvent>([&](const WindowResizeEvent& event) -> bool {
		m_RenderSystem.get_renderer().set_viewport({ 0.f, 0.f, (float)event.width, (float)event.height });
		m_MainCamera.set_orthogonal(0.f, (float)event.width, (float)event.height, 0.f, -1.f, 1.f);
		return false;
	});

	m_RenderSystem.get_renderer().on_event(event);
}
void MainView::on_render()
{
	m_RenderSystem.render();
	m_QuadRenderer.render();
}

void MainView::initialize_textures()
{
	auto& assetLibrary = r_Engine->get_asset_library();

	// Default Texture:
	{
		auto [descriptor, asset] = create_default_texture();
		auto defaultTexture = r_Engine->get_graphics_device().create_texture(descriptor, asset);
		m_RenderSystem.get_renderer().set_default_texture(defaultTexture);
	}

	// Scene Texture:
	{
		TextureDescriptor descriptor;
		descriptor.width = 640;
		descriptor.height = 480;

		m_SceneData.sceneTexture = r_Engine->get_graphics_device().create_texture(descriptor);
	}
}
void MainView::initialize_renderer_system()
{
	auto windowSize = r_Engine->get_window().data.properties.dimensions;

	m_MainCamera.set_orthogonal(0.0f, (float)windowSize.x, 0.f, (float)windowSize.y, -1.f, 1.f);

	// Main:
	m_RenderSystem.initialize(r_Engine->get_config_data(), &r_Engine->get_graphics_device(), &m_MainCamera, &scene);
	m_RenderSystem.get_renderer().set_clear_color({ 40.f / 255.f, 40.f / 255.f, 40.f / 255.f, 1.0f });
	m_RenderSystem.get_renderer().set_viewport({ 0.f, 0.f, (float)windowSize.x, (float)windowSize.y });
	m_RenderSystem.get_renderer().set_render_target_texture(m_SceneData.sceneTexture);

	// Instanced:
	m_QuadRenderer.initialize(r_Engine->get_config_data(), &r_Engine->get_graphics_device(), &m_MainCamera);
	m_QuadRenderer.set_clear_color({ 1.0f, 0.0f, 0.0f, 1.0f });
	m_QuadRenderer.set_viewport({ 0.f, 0.f, (float)windowSize.x, (float)windowSize.y });

	float size = 25.0;
	for (int x = 0; x < 10; ++x)
	{
		for (int y = 0; y < 10; ++y)
			m_QuadRenderer.add_quad({ { x * size + size, y * size, 0.0f }, { size, size }, 0 });
	}
}

void MainView::append_window_title(const std::string& extraText)
{
	auto appendedTitle = r_Engine->get_config_data().windowProperties.title + " - " + extraText;
	set_window_title(&r_Engine->get_window(), appendedTitle);
}
