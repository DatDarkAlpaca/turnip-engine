#include "pch.h"
#include "ViewSystem.h"

namespace tur
{
	ViewHandle ViewSystem::Add(tur_unique<View> view)
	{
		view->OnViewInstantiated();
		view->SetHandler(this);

		m_Views.push_back(std::move(view));

		return m_Views.size() - 1;
	}

	void ViewSystem::Remove(ViewHandle handle)
	{
		auto it = m_Views.begin() + handle;
		it->get()->OnViewRemoved();

		m_Views.erase(it);
	}

	View& ViewSystem::GetView(ViewHandle handle)
	{
		TUR_ASSERT(m_Views.size() - 1 <= handle, "Invalid view index selected");
		return *m_Views[handle].get();
	}
}