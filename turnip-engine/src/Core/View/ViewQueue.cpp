#include "pch.h"
#include "ViewQueue.h"

namespace tur
{
	ViewQueue::~ViewQueue()
	{
		for (auto& view : m_Views)
		{
			view->OnShutdown();
			delete view;
		}
		m_Views.clear();
	}

	void ViewQueue::Push(IView* view)
	{
		m_Views.push_back(view);
		view->OnInitialize();
	}

	void ViewQueue::Pop(IView* view)
	{
		auto it = std::find(m_Views.begin(), m_Views.end(), view);
		if (it != m_Views.end())
		{
			(*it)->OnShutdown();
			delete* it;
			m_Views.erase(it);
		}
	}
}