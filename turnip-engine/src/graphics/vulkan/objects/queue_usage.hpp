#pragma once
#include <vector>
#include "common.hpp"

namespace tur
{
	enum class QueueUsage : u16
	{
		NONE = 0,
		GRAPHICS = 1 << 1,
		PRESENT = 1 << 2,
		COMPUTE = 1 << 3,
		TRANSFER = 1 << 4,
		SPARSE_BINDING = 1 << 5,
	};

	inline u32 operator& (QueueUsage lhs, QueueUsage rhs)
	{
		return static_cast<u32>(lhs) & static_cast<u32>(rhs);
	}

	inline u32 operator| (QueueUsage lhs, QueueUsage rhs)
	{
		return static_cast<u32>(lhs) | static_cast<u32>(rhs);
	}

	inline QueueUsage& operator |= (QueueUsage& lhs, QueueUsage rhs)
	{
		lhs = static_cast<QueueUsage>(lhs | rhs);
		return lhs;
	}

	inline QueueUsage& operator &= (QueueUsage& lhs, QueueUsage rhs)
	{
		lhs = static_cast<QueueUsage>(lhs & rhs);
		return lhs;
	}

	inline u64 count_queue_usage_matches(QueueUsage lhs, QueueUsage rhs)
	{
		u32 overlap = lhs & rhs;

		u64 count = 0;
		while (overlap)
		{
			count += overlap & 1;
			overlap >>= 1;
		}

		return count;
	}

	inline std::vector<const char*> get_queue_usage_strings(QueueUsage usage)
	{
		std::vector<const char*> usageStrings;

		if (usage & QueueUsage::NONE)
			usageStrings.push_back("none");

		if (usage & QueueUsage::GRAPHICS)
			usageStrings.push_back("graphics");

		if (usage & QueueUsage::PRESENT)
			usageStrings.push_back("present");

		if (usage & QueueUsage::COMPUTE)
			usageStrings.push_back("compute");

		if (usage & QueueUsage::TRANSFER)
			usageStrings.push_back("transfer");

		if (usage & QueueUsage::SPARSE_BINDING)
			usageStrings.push_back("sparse_binding");

		return usageStrings;
	}

	inline QueueUsage get_queue_usage_from_string(const std::string& usageString)
	{
		std::string testString = usageString;
		to_lower(testString);

		if (testString == "none")
			return QueueUsage::NONE;

		if (testString == "graphics")
			return QueueUsage::GRAPHICS;

		if (testString == "present")
			return QueueUsage::PRESENT;

		if (testString == "compute")
			return QueueUsage::COMPUTE;

		if (testString == "transfer")
			return QueueUsage::TRANSFER;

		if (testString == "sparse_binding")
			return QueueUsage::SPARSE_BINDING;

		return QueueUsage::NONE;
	}
}