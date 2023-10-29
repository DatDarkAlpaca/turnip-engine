#include "pch.h"
#include "ViewHolder.h"

namespace tur
{
	size_t ViewHolder::AddView(tur_unique<View> view)
	{
		view->OnInstantiated();
		m_Views.push_back(std::move(view));

		return m_Views.size() - 1;
	}

	void ViewHolder::RemoveView(size_t index)
	{
		auto it = m_Views.begin() + index;
		it->get()->OnRemoved();

		m_Views.erase(it);
	}

	View& ViewHolder::GetView(size_t index)
	{
		TUR_ASSERT(m_Views.size() - 1 <= index, "Invalid view index selected");
		return *m_Views[index].get();
	}
}