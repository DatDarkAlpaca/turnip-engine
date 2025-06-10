#pragma once
#include <vector>
#include <deque>
#include "common.hpp"

namespace tur
{
	template<typename Type>
	class free_list
	{
		using iterator = Type*;
		using const_iterator = const Type*;

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

		bool constains(handle_type handle) const
		{
			return std::find(m_FreeList.begin(), m_FreeList.end(), handle) == m_FreeList.end();
		}

		void clear()
		{
			m_Data.clear();
			m_FreeList.clear();
			m_ExistsEmptySlots = false;
		}

	public:
		const_iterator cbegin() const { return m_Data.data(); }
		const_iterator cend() const { return m_Data.data() + m_Data.size(); }
		const_iterator begin() const { return cbegin(); }
		const_iterator end() const { return cend(); }

		iterator begin() { return m_Data.data(); }
		iterator end() { return m_Data.data() + m_Data.size(); }

	public:
		std::vector<Type> available() const
		{
			std::vector<Type> available;
			available.reserve(m_Data.size() - m_FreeList.size());

			for(u64 i = 0; i < m_Data.size(); ++i)
			{ 
				if (std::find(m_FreeList.begin(), m_FreeList.end(), i) != m_FreeList.end())
					continue;

				available.push_back(m_Data[i]);
			}

			return available;
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