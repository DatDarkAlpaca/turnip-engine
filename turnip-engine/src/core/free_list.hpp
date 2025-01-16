#pragma once
#include <vector>
#include <deque>
#include "common.hpp"

namespace tur
{
	template<typename Type>
	class free_list 
	{
	public:
		handle_type add(const Type& data)
		{
			if (!m_ExistsEmptySlots)
			{
				m_Data.push_back(data);
				return static_cast<handle_type>(m_Data.size() - 1);
			}

			handle_type emptyHandle = m_FreeList.back();
			m_FreeList.pop_back();

			if (m_FreeList.empty())
				m_ExistsEmptySlots = false;

			m_Data[emptyHandle] = data;
			return emptyHandle;
		}

		Type remove(handle_type handle)
		{
			m_ExistsEmptySlots = true;
			m_FreeList.push_back(handle);
			return m_Data[handle];
		}

		Type get(handle_type handle) const
		{
#ifdef TUR_DEBUG
			if (!is_valid_handle(handle))
				TUR_LOG_CRITICAL("Invalid handle: {}", handle);
#endif
			return m_Data[handle];
		}

	private:
		inline bool is_valid_handle(handle_type handle) const
		{
			for (const auto& emptyHandle : m_FreeList)
			{
				if (handle == emptyHandle)
					return false;
			}

			return true;
		}

	private:
		std::vector<Type> m_Data;
		std::deque<handle_type> m_FreeList;

		bool m_ExistsEmptySlots = false;
	};
}