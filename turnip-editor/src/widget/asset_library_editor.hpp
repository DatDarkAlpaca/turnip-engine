#pragma once
#include <common.hpp>
#include "core/scene/scene.hpp"
#include "scene_data.hpp"
#include "widget.hpp"


class AssetLibraryEditor : public Widget
{
public:
	void initialize(NON_OWNING tur::AssetLibrary* assetLibrary, NON_OWNING RendererAssemblerSystem* assemblerSystem, NON_OWNING GUISystem* guiSystem);

public:
	void on_render_gui();

	void on_event(Event& event) override;

private:
	NON_OWNING RendererAssemblerSystem* assemblerSystem = nullptr;
	NON_OWNING tur::AssetLibrary* assetLibrary = nullptr;
	NON_OWNING GUISystem* guiSystem = nullptr;

private:
	AssetType m_CurrentFilterType = AssetType::NONE;
	AssetType m_CurrentSelectedType = AssetType::NONE;

private:
	NON_OWNING TextureAsset* m_TextureAsset = nullptr;
	texture_handle m_CurrentHandle = invalid_handle;

private:
	constexpr static inline float s_LeftPanelProportion = 0.3f;
	constexpr static inline float s_RightPanelProportion = 1 - s_LeftPanelProportion;

	constexpr static inline float s_UpperPanelPropotion = 0.5f;
	constexpr static inline float s_LowerPanelPropotion = 1 - s_UpperPanelPropotion;
};