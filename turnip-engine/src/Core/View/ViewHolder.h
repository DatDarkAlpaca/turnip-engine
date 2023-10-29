#pragma once
#include <vector>
#include "View.h"
#include "Common.h"

namespace tur
{
	class ViewHolder
	{
		using ViewsType = std::vector<tur_unique<View>>;

	public:
		size_t AddView(tur_unique<View> view);

		void RemoveView(size_t index);

		View& GetView(size_t index);

	public:
		ViewsType::reverse_iterator rbegin() { return m_Views.rbegin(); }
		ViewsType::reverse_iterator rend() { return m_Views.rend(); }
		ViewsType::iterator begin() { return m_Views.begin(); }
		ViewsType::iterator end() { return m_Views.end(); }

		ViewsType::const_reverse_iterator rbegin() const { return m_Views.rbegin(); }
		ViewsType::const_reverse_iterator rend() const { return m_Views.rend(); }
		ViewsType::const_iterator begin() const { return m_Views.begin(); }
		ViewsType::const_iterator end()	const { return m_Views.end(); }

	private:
		std::vector<tur_unique<View>> m_Views;
	};
}