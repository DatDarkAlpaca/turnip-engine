#include "pch.hpp"
#include "asset_library_editor.hpp"

void AssetLibraryEditor::initialize(NON_OWNING tur::AssetLibrary* assetLibrary, NON_OWNING RendererAssemblerSystem* assemblerSystem, NON_OWNING GUISystem* guiSystem)
{
	this->guiSystem = guiSystem;
	this->assetLibrary = assetLibrary;
	this->assemblerSystem = assemblerSystem;
}

void AssetLibraryEditor::on_render_gui()
{
	ImGui::Begin("Asset Library Editor");
	ImVec2 windowSize = ImGui::GetWindowSize();

	// Left Panel:
	{
		ImGui::BeginChild("Asset List", ImVec2(windowSize.x * s_LeftPanelProportion, windowSize.y - 40), true);
		
		if (ImGui::BeginCombo("Asset Type", get_asset_type_name(m_CurrentFilterType)))
		{
			if (ImGui::Selectable("All"))
				m_CurrentFilterType = AssetType::NONE;

			if (ImGui::Selectable("Texture"))
				m_CurrentFilterType = AssetType::TEXTURE;

			ImGui::EndCombo();
		}
		
		if (m_CurrentFilterType == AssetType::NONE || m_CurrentFilterType == AssetType::TEXTURE)
		{
			for (auto& texture : assetLibrary->textures)
			{
				if (ImGui::Button(texture.filepath.string().c_str()))
				{
					m_TextureAsset = &texture;
					m_CurrentHandle = assemblerSystem->textureMap.at(texture.uuid);
					m_CurrentSelectedType = AssetType::TEXTURE;
				}
			}
		}
		
		ImGui::EndChild();
	}

	ImGui::SameLine();

	// Right Panel:
	{
		float rightPanelWidth = windowSize.x * s_RightPanelProportion;

		ImGui::BeginChild("Asset Information:", ImVec2(rightPanelWidth, windowSize.y - 40), true);
		float rightPanelHeight = ImGui::GetContentRegionAvail().y;
		
		// Upper:
		{
			ImGui::BeginChild("Asset Display", ImVec2(rightPanelWidth, windowSize.y * s_UpperPanelPropotion), true, ImGuiWindowFlags_NoScrollbar);
			ImVec2 assetDisplaySize = ImGui::GetContentRegionAvail();
			constexpr float padding = 50.f;

			ImGui::Text("Asset Display Area");
			ImGui::Separator();

			if (m_CurrentHandle != invalid_handle)
			{
				float windowWidth = assetDisplaySize.x;
				float windowHeight = assetDisplaySize.y;

				float textureWidth = m_TextureAsset->width;
				float textureHeight = m_TextureAsset->height;
				float aspectRatio = textureWidth / textureHeight;

				float usedWidth;
				float usedHeight;

				if (windowHeight < windowWidth)
				{
					usedWidth = assetDisplaySize.y * aspectRatio;
					usedHeight = assetDisplaySize.y;
				}
				else 
				{
					float usedWidth = assetDisplaySize.x;
					float usedHeight = assetDisplaySize.x / aspectRatio;
				}

				guiSystem->texture(m_CurrentHandle, { usedWidth, usedHeight });
			}
			
			ImGui::EndChild();
		}

		// Bottom:
		{
			ImGui::BeginChild("Asset Information", ImVec2(rightPanelWidth, windowSize.y * s_LowerPanelPropotion), true);

			ImGui::Text("Asset Information");
			ImGui::Separator();

			switch (m_CurrentSelectedType)
			{
				case AssetType::TEXTURE:
				{
					const auto& texture = m_TextureAsset;

					ImGui::Text("UUID: %lx", texture->uuid);
					ImGui::Text("Filepath: %s", texture->filepath.string().c_str());
					ImGui::Text("Data Format: %s", get_texture_data_format_name(texture->dataFormat));

					ImGui::Text("Dimensions: (%lupx, %lupx)", texture->width, texture->height);
					ImGui::Text("Depth: %lu", texture->depth);
					ImGui::Text("Channels: %lux", texture->channels);

					ImGui::Text("Offset: (%lu, %lu, %lu)", texture->xOffset, texture->yOffset, texture->zOffset);
					ImGui::Text("Float Texture: %s", texture->floatTexture ? "Yes" : "No");
				} break;
			}
			
			ImGui::EndChild();
		}

		ImGui::EndChild();
	}

	ImGui::End();
}

void AssetLibraryEditor::on_event(Event& event)
{
}
