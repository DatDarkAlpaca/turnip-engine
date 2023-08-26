#pragma once
#include <sstream>
#include <string>
#include "Random/Random.h"

namespace tur
{
	// Todo: generate 64-bit number instead of a string. 
	// This solution was made for testing purposes only.
	class UUID
	{
	public:
		static std::string UUID4()
		{
			std::stringstream ss;
		
			ss << std::hex;
			for (size_t i = 0; i < 8; ++i)
				ss << Random::GetInt(0, 15);

			ss << '-';
			for (size_t i = 0; i < 4; ++i)
				ss << Random::GetInt(0, 15);
			
			ss << '-4';
			for (size_t i = 0; i < 3; ++i)
				ss << Random::GetInt(0, 15);

			ss << '-';
			ss << Random::GetInt(8, 11);
			for (size_t i = 0; i < 3; ++i)
				ss << Random::GetInt(0, 15);

			ss << '-';
			for (size_t i = 0; i < 12; ++i)
				ss << Random::GetInt(0, 15);

			return ss.str();
		}
	};
}