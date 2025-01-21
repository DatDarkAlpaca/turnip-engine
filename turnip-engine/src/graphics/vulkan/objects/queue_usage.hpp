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

		GENERAL_PRESENT = GRAPHICS | PRESENT,
		ALL = GRAPHICS | PRESENT | COMPUTE | TRANSFER | SPARSE_BINDING
	};

	inline u64 count_queue_usage_matches(QueueUsage lhs, QueueUsage rhs)
	{
		u16 overlap = static_cast<uint16_t>(lhs) & static_cast<uint16_t>(rhs);

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

		if (static_cast<u16>(usage) & static_cast<u16>(QueueUsage::NONE))
			usageStrings.push_back("none");

		if (static_cast<u16>(usage) & static_cast<u16>(QueueUsage::GRAPHICS))
			usageStrings.push_back("graphics");

		if (static_cast<u16>(usage) & static_cast<u16>(QueueUsage::PRESENT))
			usageStrings.push_back("present");

		if (static_cast<u16>(usage) & static_cast<u16>(QueueUsage::COMPUTE))
			usageStrings.push_back("compute");

		if (static_cast<u16>(usage) & static_cast<u16>(QueueUsage::TRANSFER))
			usageStrings.push_back("transfer");

		if (static_cast<u16>(usage) & static_cast<u16>(QueueUsage::SPARSE_BINDING))
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
	}
}