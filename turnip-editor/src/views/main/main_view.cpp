#include "pch.hpp"
#include "main_view.hpp"
#include "event/events.hpp"

#include <core/scene/scene_serialization.hpp>

using namespace tur;

MainView::MainView(const ProjectData& projectData)
	: m_ProjectData(projectData)
{
	m_MainMenuBar.initialize(this);

	m_EntityInspector.set_callback(BIND(&MainView::on_event, this));
	m_SceneEditor.set_callback(BIND(&MainView::on_event, this));
	m_MainMenuBar.set_callback(BIND(&MainView::on_event, this));
	m_SceneViewer.set_callback(BIND(&MainView::on_event, this));
}

void MainView::set_project_data(const ProjectData& projectData)
{
	m_ProjectData = projectData;

	// Reset:
	scene.get_registry().clear();
	m_SceneData.viewerSelectedEntity = Entity();

	ScriptSystem::set_project(m_ProjectData);

	// Deserialize main scene:
	if (!std::filesystem::is_regular_file(projectData.projectPath / "scene.json"))
		return;

	SceneDeserializer deserializer(&scene, projectData.projectPath / "scene.json");
	deserializer.deserialize();

	// Load assets:
	for (const auto& [entity, textureComponent] : scene.get_registry().view<TextureComponent>().each())
	{
		auto* assetLibrary = &engine->assetLibrary;
		auto* graphicsDevice = &engine->graphicsDevice;

		engine->workerPool.submit<AssetInformation>([assetLibrary, textureComponent]() {
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

	// Widgets:
	m_EntityInspector.initialize(engine, &scene, &m_SceneData);
	m_SceneViewer.initialize(&scene, &m_SceneData);
	m_SceneEditor.initialize(&engine->graphicsDevice, &m_SceneData);

	initialize_renderer_system();
	initialize_textures();
}
void MainView::on_update()
{
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

	subscriber.subscribe<OnProjectEdited>([&](OnProjectEdited&) -> bool {
		append_window_title(m_ProjectData.projectName + "*");
		return false;
	});

	subscriber.subscribe<OnProjectSaved>([&](OnProjectSaved&) -> bool {
		append_window_title(m_ProjectData.projectName);
		return false;
	});

	subscriber.subscribe<OnEntityInspectorInitialize>([&](const OnEntityInspectorInitialize&) -> bool {
		TUR_LOG_INFO("Inspector initialized");
		return true;
	});

	subscriber.subscribe<SceneEditorResized>([&](const SceneEditorResized& resizeEvent) -> bool {
		if (resizeEvent.width <= 0 || resizeEvent.height <= 0)
			return false;
		
		// Render target:
		update_render_target(resizeEvent.width, resizeEvent.height);
		
		// Viewport:
		if(r_QuadRenderer)
			renderer_set_viewport(r_QuadRenderer, { 0.0f, 0.0f, (float)resizeEvent.width, (float)resizeEvent.height });
		
		// Camera:
		m_SceneData.editorCamera.camera.set_orthogonal(0.f, (float)resizeEvent.width, 0.f, (float)resizeEvent.height, -1.f, 1.f);
		return false;
	});

	subscriber.subscribe<SceneEditorClicked>([&](const SceneEditorClicked& clickEvent) -> bool {
		// Select object on view:
		return false;
	});

	m_EntityInspector.on_event(event);
	m_SceneEditor.on_event(event);
}
void MainView::on_render()
{
	quad_renderer_system_begin(engine->quadRendererSystem);
	quad_renderer_system_render(engine->quadRendererSystem, m_SceneData.sceneRenderTarget);
	quad_renderer_system_end(engine->quadRendererSystem);

	//instanced_quad_system_begin(engine->instancedQuadSystem);
	//instanced_quad_system_render(engine->instancedQuadSystem, m_SceneData.sceneRenderTarget);
	//instanced_quad_system_end(engine->instancedQuadSystem);
}

void MainView::initialize_textures()
{
	auto& assetLibrary = engine->assetLibrary;

	// Default Texture:
	{
		auto [descriptor, asset] = create_default_texture();
		auto defaultTexture = engine->graphicsDevice.create_texture(descriptor, asset);

		quad_renderer_set_default_texture(engine->quadRendererSystem.renderer, defaultTexture);
	}
}
void MainView::initialize_renderer_system()
{
	r_InstancedRenderer = &engine->instancedQuadSystem.renderer;
	r_QuadRenderer = &engine->quadRendererSystem.renderer;

	// Main:
	{
		quad_renderer_system_set_scene(engine->quadRendererSystem, &scene);
		quad_renderer_system_set_camera(engine->quadRendererSystem, &m_SceneData.editorCamera.camera);

		renderer_set_clear_color(r_QuadRenderer, { 40, 40, 40, 255 });			
	}

	// Instanced:
	{
		auto windowSize = engine->window.data.properties.dimensions;

		instanced_quad_system_set_scene(engine->instancedQuadSystem, &scene);
		instanced_quad_system_set_camera(engine->instancedQuadSystem, &m_SceneData.editorCamera.camera);
		
		renderer_set_clear_color(r_InstancedRenderer, { 40, 40, 40, 255 });
		renderer_set_viewport(r_InstancedRenderer, { 0.f, 0.f, (float)windowSize.x, (float)windowSize.y });
		renderer_set_should_clear(r_InstancedRenderer, false);
	}
}

void MainView::append_window_title(const std::string& extraText)
{
	auto appendedTitle = engine->configData.windowProperties.title + " - " + extraText;
	set_window_title(&engine->window, appendedTitle);
}

void MainView::update_render_target(u32 width, u32 height)
{
	if(m_SceneData.sceneRenderTarget != invalid_handle)
		engine->graphicsDevice.destroy_render_target(m_SceneData.sceneRenderTarget);

	RenderTargetDescriptor descriptor;
	{
		descriptor.colorAttachments.push_back(m_SceneData.sceneTexture);
		descriptor.width = width;
		descriptor.height = height;
	}

	m_SceneData.sceneRenderTarget = engine->graphicsDevice.create_render_target(descriptor);
}