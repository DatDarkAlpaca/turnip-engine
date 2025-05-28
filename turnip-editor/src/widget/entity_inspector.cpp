#include "pch.hpp"
#include "entity_inspector.hpp"
#include "event/events.hpp"

void EntityInspector::initialize(NON_OWNING tur::TurnipEngine* engine, NON_OWNING tur::Scene* scene, SceneData* sceneData)
{
	r_Engine = engine;
	r_Scene = scene;
	r_SceneData = sceneData;

	OnEntityInspectorInitialize initEvent;
	callback(initEvent);
}
void EntityInspector::on_render_gui()
{
	ImGui::Begin("Entity Inspector");

	if (r_SceneData->viewerSelectedEntity.get_handle() == entt::null)
		return ImGui::End();

	render_components(r_SceneData->viewerSelectedEntity);

	ImGui::Separator();

	render_component_add_list(r_SceneData->viewerSelectedEntity);

	ImGui::End();
}

void EntityInspector::on_event(Event& event)
{
	Subscriber subscriber(event);
	subscriber.subscribe<SceneEditorResized>([&](SceneEditorResized& resizeEvent) -> bool {
		m_EditorArea.width = (float)resizeEvent.width;
		m_EditorArea.height = (float)resizeEvent.height;

		return false;
	});

	subscriber.subscribe<SceneEditorMoved>([&](SceneEditorMoved& moveEvent) -> bool {
		m_EditorArea.x = (float)moveEvent.x;
		m_EditorArea.y = (float)moveEvent.y;

		return false;
	});
}

void EntityInspector::render_components(Entity selectedEntity)
{
	if (selectedEntity.has_component<TransformComponent>())
		render_transform_component(selectedEntity);

	if (selectedEntity.has_component<TextureComponent>())
		render_texture_component(selectedEntity);

	if (selectedEntity.has_component<EntityScriptsComponent>())
	{
		if (!selectedEntity.get_component<EntityScriptsComponent>().scriptComponents.empty())
			render_script_component(selectedEntity);
	}
}
void EntityInspector::render_transform_component(Entity selectedEntity)
{
	auto& transform = selectedEntity.get_component<TransformComponent>().transform;

	if (ImGui::CollapsingHeader("Transform"))
	{
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::DragFloat3("##position", &transform.position[0]);
		ImGui::PopItemWidth();

		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::DragFloat3("##rotation", &transform.rotation[0]);
		ImGui::PopItemWidth();

		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::DragFloat3("##scale", &transform.scale[0]);
		ImGui::PopItemWidth();
	}
}
void EntityInspector::render_texture_component(Entity selectedEntity)
{
	auto* textureComponent = &selectedEntity.get_component<TextureComponent>();
	const auto& textures = r_Engine->graphicsDevice.get_textures();

	auto* assetLibrary = &r_Engine->assetLibrary;
	auto* graphicsDevice = &r_Engine->graphicsDevice;
	
	auto entityID = selectedEntity.get_handle();
	auto* scene = r_Scene;

	if (ImGui::CollapsingHeader("Texture"))
	{
		// Disallow use of internal graphics data
		if (ImGui::ImageButton((void*)textures.get(textureComponent->handle).handle, { 50.0f, 50.0f }))
		{
			auto filepaths = open_file_dialog("Open texture", { "All Images (*.png, *.jpg)", "*.png", "*.jpg" });

			if (filepaths.empty())
				return;

			auto& filepath = std::filesystem::path(filepaths[0]);

			r_Engine->workerPool.submit<AssetInformation>([assetLibrary, filepath]() {
				return load_texture_asset(assetLibrary, filepath);
			}, [textureComponent, filepath, assetLibrary, graphicsDevice](AssetInformation information) {

				if (information.isDuplicate)
					return;

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
				textureComponent->handle = textureGraphics;
				textureComponent->filepath = filepath;
			});
		}
	}
}
void EntityInspector::render_script_component(Entity selectedEntity)
{
	auto& scripts = selectedEntity.get_component<EntityScriptsComponent>().scriptComponents;

	for (const auto& script : scripts)
	{
		if (ImGui::CollapsingHeader(script.className.c_str()))
		{
			std::string buttonName = script.className + "##ExplorerButton";
			if (ImGui::Button(buttonName.c_str()))
				open_in_explorer(script.filepath);
		}
	}
}

void EntityInspector::render_component_add_list(Entity selectedEntity)
{
	ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeight()));
	if (ImGui::Button("Add Component", ImVec2(-FLT_MIN, 0)))
		ImGui::OpenPopup("AddComponentPopup");

	if (ImGui::BeginPopup("AddComponentPopup"))
	{
		render_add_transform_component(selectedEntity);

		render_add_texture_component(selectedEntity);

		render_add_script_component(selectedEntity);

		ImGui::EndPopup();
	}
}

void EntityInspector::render_add_transform_component(Entity selectedEntity)
{
	if (!selectedEntity.has_component<TransformComponent>() && ImGui::MenuItem("Transform"))
	{
		selectedEntity.add_component<TransformComponent>();
		r_SceneData->projectEdited = true;
	}
}
void EntityInspector::render_add_texture_component(Entity selectedEntity)
{
	if (!selectedEntity.has_component<TextureComponent>() && ImGui::MenuItem("Texture2D"))
	{
		selectedEntity.add_component<TextureComponent>();
		r_SceneData->projectEdited = true;
	}
}
void EntityInspector::render_add_script_component(Entity selectedEntity)
{
	if (ImGui::MenuItem("Entity Script"))
	{
		using namespace std::filesystem;

		auto scriptFilepath = path(save_file_dialog("New Script", "", { "Script files (.cs)", "*.cs" }));
		if (scriptFilepath.empty())
			return;

		path correctedPath = scriptFilepath.replace_extension(".cs");

		std::string className = scriptFilepath.filename().replace_extension("").string();
		create_empty_script(correctedPath, className);

		InternalEntityScript script;
		{
			script.className = className;
			script.filepath = correctedPath;
			selectedEntity.get_component<EntityScriptsComponent>().scriptComponents.push_back(script);
		}

		r_SceneData->projectEdited = true;
	}
}