#include "pch.h"
#include "Color.h"

static glm::vec4 ConvertHexToRGBA(const std::string& hexValue)
{
	std::string sanitizedValue = hexValue;
	if (hexValue[0] == '#')
		sanitizedValue.erase(0, 1);

	glm::vec4 result = glm::vec4(0.f);

	U64 convertedHex;
	std::stringstream ss;
	ss << std::hex << sanitizedValue;
	ss >> convertedHex;

	result.r = double(((convertedHex >> 24) & 0xFF) / 255.);
	result.g = double(((convertedHex >> 16) & 0xFF) / 255.);
	result.b = double(((convertedHex >> 8) & 0xFF) / 255.);
	result.a = double(((convertedHex >> 0) & 0xFF) / 255.);

	return result;
}

static glm::vec4 ConvertHexToRGBA(U64 hexValue)
{
	glm::vec4 result = glm::vec4(0.f);

	result.r = double(((hexValue >> 24) & 0xFF) / 255.);
	result.g = double(((hexValue >> 16) & 0xFF) / 255.);
	result.b = double(((hexValue >> 8) & 0xFF) / 255.);
	result.a = double(((hexValue >> 0) & 0xFF) / 255.);

	return result;
}

namespace tur
{
	Color::Color(double r, double g, double b, double a)
		: m_Color{ r, g, b, a }
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

	Color::Color(U32 r, U32 g, U32 b, U32 a)
		: m_Color { r / 255., g / 255. , b / 255. , a / 255. }
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

	Color::Color(int r, int g, int b, int a)
		: m_Color({ r / 255., g / 255., b / 255., a / 255. })
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

	Color::Color(const char* hex)
	{
		m_Color = ConvertHexToRGBA(hex);
	}

	Color::Color(U64 hex)
	{
		m_Color = ConvertHexToRGBA(hex);
	}

	glm::vec4 Color::RGBA() const
	{
		return { U32(m_Color.r * 255), U32(m_Color.g * 255),  U32(m_Color.b * 255), U32(m_Color.a * 255) };
	}
}