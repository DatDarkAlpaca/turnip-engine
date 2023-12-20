#include "pch.h"
#include "ViewSystem.h"

namespace tur
{
	size_t ViewSystem::AddView(tur_unique<View> view)
	{
		view->OnInstantiated();
		m_Views.push_back(std::move(view));

		return m_Views.size() - 1;
	}

	void ViewSystem::RemoveView(size_t index)
	{
		auto it = m_Views.begin() + index;
		it->get()->OnRemoved();

		m_Views.erase(it);
	}

	View& ViewSystem::GetView(size_t index)
	{
		TUR_ASSERT(m_Views.size() - 1 <= index, "Invalid view index selected");
		return *m_Views[index].get();
	}
}