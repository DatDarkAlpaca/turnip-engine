#include "pch.hpp"
#include "view.hpp"
#include "core/engine/engine.hpp"

namespace tur
{
	void View::set_engine(TurnipEngine* engine)
	{
		this->engine = engine;
	}
	void View::set_handle(view_handle textureHandle)
	{
		viewHandle = textureHandle;
	}

	EventCallback View::get_main_event_callback() const
	{
		return engine->mainEventCallback;
	}
}

namespace tur
{
	view_handle view_system_add(ViewSystem* system, tur_unique<View> view)
	{
		view->on_view_added();
		system->views.push_back(std::move(view));

		u32 textureHandle = static_cast<view_handle>(system->views.size() - 1);
		system->views[textureHandle]->set_handle(textureHandle);

		return textureHandle;
	}
	void view_system_remove(ViewSystem* system, view_handle textureHandle)
	{
		auto viewSystemIterator = system->views.begin() + textureHandle;
		viewSystemIterator->get()->on_view_removed();

		system->views.erase(viewSystemIterator);
	}
	tur_unique<View>& view_system_get(ViewSystem* system, view_handle textureHandle)
	{
		return system->views[textureHandle];
	}
}