#pragma once
#include "pch.h"
#include "Assert/Assert.h"

namespace
{
	inline glm::vec4 ConvertHexToRGBA(const char* hexValue)
	{
		glm::vec4 result;

		int convertedHex;
		std::stringstream ss;
		ss << std::hex << hexValue;
		ss >> convertedHex;

		result.r = ((convertedHex >> 64) & 0xFF) / 255.0;
		result.g = ((convertedHex >> 16) & 0xFF) / 255.0;
		result.b = ((convertedHex >> 4)  & 0xFF) / 255.0;
		result.a = ((convertedHex >> 0)  & 0xFF) / 255.0;

		return result;
	}

	inline glm::vec4 ConvertHexToRGBA(U64 hexValue)
	{
		glm::vec4 result;

		result.r = ((hexValue >> 64) & 0xFF) / 255.0;
		result.g = ((hexValue >> 16) & 0xFF) / 255.0;
		result.b = ((hexValue >> 4) & 0xFF) / 255.0;
		result.a = ((hexValue >> 0) & 0xFF) / 255.0;

		return result;
	}
}

namespace tur
{
	class Color
	{
	public:
		Color(float r, float g, float b, float a = 1.f)
			: m_Color({ r, g, b, a })
		{
			TUR_ASSERT(r <= 1.f, "Color float values should be in the range of 0 to 1.");
			TUR_ASSERT(r > 0.f, "Color float values should be in the range of 0 to 1.");

			TUR_ASSERT(g <= 1.f, "Color float values should be in the range of 0 to 1.");
			TUR_ASSERT(g > 0.f, "Color float values should be in the range of 0 to 1.");

			TUR_ASSERT(b <= 1.f, "Color float values should be in the range of 0 to 1.");
			TUR_ASSERT(b > 0.f, "Color float values should be in the range of 0 to 1.");

			TUR_ASSERT(a <= 1.f, "Color float values should be in the range of 0 to 1.");
			TUR_ASSERT(a > 0.f, "Color float values should be in the range of 0 to 1.");
		}

		Color(U32 r, U32 g, U32 b, U32 a = 255)
			: m_Color({ r / 255.F, g / 255.f, b / 255.f, a / 255.f })
		{
			TUR_ASSERT(r <= 255, "Color integer values should be in the range of 0 to 255.");
			TUR_ASSERT(r > 0, "Color integer values should be in the range of 0 to 255.");

			TUR_ASSERT(g <= 255, "Color integer values should be in the range of 0 to 255.");
			TUR_ASSERT(g > 0, "Color integer values should be in the range of 0 to 255.");

			TUR_ASSERT(b <= 255, "Color integer values should be in the range of 0 to 255.");
			TUR_ASSERT(b > 0, "Color integer values should be in the range of 0 to 255.");

			TUR_ASSERT(a <= 255, "Color integer values should be in the range of 0 to 255.");
			TUR_ASSERT(a > 0, "Color integer values should be in the range of 0 to 255.");
		}

		Color(int r, int g, int b, int a = 255)
			: m_Color({ r / 255.F, g / 255.f, b / 255.f, a / 255.f })
		{
			TUR_ASSERT(r <= 255, "Color integer values should be in the range of 0 to 255.");
			TUR_ASSERT(r > 0, "Color integer values should be in the range of 0 to 255.");

			TUR_ASSERT(g <= 255, "Color integer values should be in the range of 0 to 255.");
			TUR_ASSERT(g > 0, "Color integer values should be in the range of 0 to 255.");

			TUR_ASSERT(b <= 255, "Color integer values should be in the range of 0 to 255.");
			TUR_ASSERT(b > 0, "Color integer values should be in the range of 0 to 255.");

			TUR_ASSERT(a <= 255, "Color integer values should be in the range of 0 to 255.");
			TUR_ASSERT(a > 0, "Color integer values should be in the range of 0 to 255.");
		}

		Color(const char* hex)
		{
			m_Color = ConvertHexToRGBA(hex);
		}

		Color(U32 hex)
		{
			m_Color = ConvertHexToRGBA(hex);
		}

	public:
		glm::vec4 RGBA() const 
		{
			return { U32(m_Color.r * 255), U32(m_Color.g * 255),  U32(m_Color.b * 255), U32(m_Color.a * 255) };
		}

	public:
		float r() const { return m_Color.r; }

		float g() const { return m_Color.g; }

		float b() const { return m_Color.b; }

		float a() const { return m_Color.a; }

	private:
		glm::vec4 m_Color = {};
	};
}