#pragma once
#include "Common.hpp"
#include "Core/Event/Event.hpp"

namespace tur
{
	class TurnipEngine;

	class WorkerPool;
	class ViewSystem;
	class AssetLibrary;

	class View
	{
	public:
		virtual ~View() = default;

	public:
		virtual void OnEngineStartup() { };

		virtual void OnViewInstantiated() { };

		virtual void OnRender() { };

		virtual void OnRenderGUI() { };

		virtual void OnUpdate() { };

		virtual void OnEvent(Event& event) { };

		virtual void OnViewRemoved() { };

		virtual void OnEngineShutdown() { };

	public:
		void SetEngine(TurnipEngine* engine) { this->engine = engine; }

		void SetHandler(ViewSystem* handler) { this->handler = handler; }

	public:
		AssetLibrary& AssetLibrary() const;

		WorkerPool& WorkerPool() const;

	protected:
		NON_OWNING TurnipEngine* engine = nullptr;
		NON_OWNING ViewSystem* handler = nullptr;
	};
}