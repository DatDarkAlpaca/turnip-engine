#pragma once
#include <vector>
#include <deque>

#include "Common.hpp"

namespace tur
{
	using default_handle_t = uint32_t;

	template<typename Type, typename HandleType = default_handle_t>
	class HandleArray
	{
	public:
		HandleType add(const Type& data)
		{
			if (!m_ExistsEmptySlots)
			{
				m_Data.push_back(data);
				return static_cast<HandleType>(m_Data.size() - 1);
			}

			HandleType emptyHandle = m_EmptyData.back();
			m_EmptyData.pop_back();

			if (m_EmptyData.empty())
				m_ExistsEmptySlots = false;

			m_Data[emptyHandle] = data;
			return emptyHandle;
		}

		Type get(HandleType handle) const
		{
			if (!isValidHandle(handle))
				TUR_LOG_CRITICAL("Invalid handle");

			return m_Data[handle];
		}

		void remove(HandleType handle)
		{
			m_ExistsEmptySlots = true;
			m_EmptyData.push_back(handle);
			m_Data[handle] = {};
		}

	public:
		bool isValidHandle(HandleType handle) const
		{
			for (const auto& emptyHandle : m_EmptyData)
			{
				if (handle == emptyHandle)
					return false;
			}

			return true;
		}

	public:
		std::vector<Type> data() const { return m_Data; }

	private:
		std::vector<Type> m_Data;
		std::deque<HandleType> m_EmptyData;
		bool m_ExistsEmptySlots = false;
	};
}