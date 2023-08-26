#pragma once
#include <random>

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
			auto distribution = std::uniform_int_distribution<std::mt19937::result_type>(minimum, maximum);
			return distribution(s_Engine);
		}

	private:
		static inline std::random_device s_Device;
		static inline std::mt19937 s_Engine { s_Device() };
	};
}