#pragma once
#include <vector>

#include "common.hpp"
#include "core/scene/scene.hpp"
#include "core/event/event.hpp"

namespace tur
{
	using view_handle = handle_type;

	class View
	{
	public:
		virtual ~View() = default;

	public:
		void set_engine(struct TurnipEngine* engine)
		{
			this->engine = engine;
		}
		void set_handle(view_handle handle)
		{
			viewHandle = handle;
		}

	public:
		virtual void on_engine_startup() { };
		virtual void on_engine_shutdown() { };

		virtual void on_view_added() { };
		virtual void on_view_removed() { };

		virtual void on_render() { };
		virtual void on_render_gui() { };

		virtual void on_update() { };

		virtual void on_event(Event& event) { };

	protected:
		NON_OWNING struct TurnipEngine* engine = nullptr;
		view_handle viewHandle = invalid_handle;
		Scene scene;
	};
}
namespace tur
{
	struct ViewSystem
	{
		std::vector<tur_unique<View>> views;
	};
	
	inline view_handle view_system_add(ViewSystem* system, tur_unique<View> view)
	{
		view->on_view_added();
		system->views.push_back(std::move(view));

		u32 handle = static_cast<view_handle>(system->views.size() - 1);
		system->views[handle]->set_handle(handle);

		return handle;
	}

	inline void view_system_remove(ViewSystem* system, view_handle handle)
	{
		auto viewSystemIterator = system->views.begin() + handle;
		viewSystemIterator->get()->on_view_removed();

		system->views.erase(viewSystemIterator);
	}

	inline tur_unique<View>& view_system_get(ViewSystem* system, view_handle handle)
	{
		return system->views[handle];
	}
}