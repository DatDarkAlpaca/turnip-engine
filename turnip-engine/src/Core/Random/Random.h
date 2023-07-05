#pragma once
#include "pch.h"

namespace tur
{
	class Random
	{
	public:
		static void Initialize()
		{
			s_Engine.seed(std::random_device()());
		}

		static int GetInt(int minimum, int maximum)
		{
			return s_DistributionInt(minimum, maximum);
		}

	private:
		static inline std::mt19937 s_Engine;
		static inline std::uniform_int_distribution<std::mt19937::result_type> s_DistributionInt;
	};
}