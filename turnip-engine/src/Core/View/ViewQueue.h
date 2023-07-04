#pragma once
#include "pch.h"
#include "View.h"

namespace tur
{
	class ViewQueue
	{
		using ViewsType = std::vector<IView*>;

	public:
		ViewQueue() = default;

		~ViewQueue()
		{
			for (auto& view : m_Views)
			{
				view->OnShutdown();
				delete view;
			}
			m_Views.clear();
		}

	public:
		void Push(IView* view)
		{
			view->OnInitialize();
			m_Views.push_back(view);
		}

		void Pop(IView* view)
		{
			auto it = std::find(m_Views.begin(), m_Views.end(), view);
			if (it != m_Views.end())
			{
				(*it)->OnShutdown();
				delete *it;
				m_Views.erase(it);
			}
		}

	public:
		ViewsType::iterator begin() { return m_Views.begin(); }
		ViewsType::iterator end() { return m_Views.end(); }
		ViewsType::reverse_iterator rbegin() { return m_Views.rbegin(); }
		ViewsType::reverse_iterator rend() { return m_Views.rend(); }

		ViewsType::const_iterator begin() const { return m_Views.begin(); }
		ViewsType::const_iterator end()	const { return m_Views.end(); }
		ViewsType::const_reverse_iterator rbegin() const { return m_Views.rbegin(); }
		ViewsType::const_reverse_iterator rend() const { return m_Views.rend(); }

	private:
		std::vector<IView*> m_Views;
	};
}