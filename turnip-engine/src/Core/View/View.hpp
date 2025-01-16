#pragma once
#include <vector>

#include "common.hpp"
#include "core/event/event.hpp"

namespace tur
{
	using view_handle = handle_type;

	class View
	{
	public:
		virtual ~View() = default;

	public:
		virtual void on_engine_startup() { };

		virtual void on_view_added() { };

		virtual void on_render() { };

		virtual void on_render_gui() { };

		virtual void on_update() { };

		virtual void on_event(Event& event) { };

		virtual void on_view_removed() { };

		virtual void on_engine_shutdown() { };
	};


	struct ViewSystem
	{
		std::vector<tur_unique<View>> views;
	};

	view_handle add_view(ViewSystem* system, tur_unique<View> view)
	{
		view->on_view_added();

		system->views.push_back(std::move(view));
		return system->views.size() - 1;
	}

	void remove_view(ViewSystem* system, view_handle handle)
	{
		auto viewSystemIterator = system->views.begin() + handle;
		viewSystemIterator->get()->on_view_removed();

		system->views.erase(viewSystemIterator);
	}

	tur_unique<View>& get_view(ViewSystem* system, view_handle handle)
	{
		return system->views[handle];
	}
}