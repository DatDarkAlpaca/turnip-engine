#pragma once
#include "View.h"

namespace tur
{
	class ViewQueue
	{
		using ViewsType = std::vector<IView*>;

	public:
		ViewQueue() = default;

		~ViewQueue();

	public:
		void AddView(IView* view);

		void RemoveView(IView* view);

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