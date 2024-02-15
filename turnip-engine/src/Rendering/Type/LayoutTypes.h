#pragma once
#include <numeric>
#include "Common.h"

namespace tur
{
	enum class LayoutType
	{
        NONE = 0,
        INT_8,
        INT_16,
        INT_32,
        INT_64,
        UINT_8,
        UINT_16,
        UINT_32,
        UINT_64,
        FLOAT_32,
        FLOAT_64,
	};

    constexpr inline uint64_t GetLayoutTypeSize(LayoutType type)
    {
        switch (type)
        {
            case LayoutType::NONE:
            {
                TUR_LOG_WARN("Invalid layout type used: NONE. Input Layout may be ");
                return 0;
            } break;

            case LayoutType::INT_8: 
                return sizeof(int8_t);

            case LayoutType::INT_16: 
                return sizeof(int16_t);

            case LayoutType::INT_32: 
                return sizeof(int32_t);

            case LayoutType::INT_64:
                return sizeof(int64_t);

            case LayoutType::UINT_8: 
                return sizeof(uint8_t);

            case LayoutType::UINT_16: 
                return sizeof(uint16_t);

            case LayoutType::UINT_32: 
                return sizeof(uint32_t);

            case LayoutType::UINT_64:
                return sizeof(uint64_t);

            case LayoutType::FLOAT_32: 
                return sizeof(float_t);

            case LayoutType::FLOAT_64: 
                return sizeof(double_t);
        }
    }
}