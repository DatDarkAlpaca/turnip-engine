#include "pch.h"
#include "Color.h"

static glm::vec4 ConvertHexToRGBA(const std::string& hexValue)
{
	TUR_ASSERT(hexValue.size() >= 6 || hexValue.size() <= 9, "Invalid HEX color used.");

	glm::vec4 result = glm::vec4(0.f);

	std::string sanitizedValue = hexValue;
	if (hexValue[0] == '#')
		sanitizedValue.erase(0, 1);

	U64 convertedHex;
	std::stringstream ss;
	ss << std::hex << sanitizedValue;
	ss >> convertedHex;

	if (sanitizedValue.length() == 6)
	{
		result.r = double(((convertedHex >> 16) & 0xFF) / 255.);
		result.g = double(((convertedHex >>  8) & 0xFF) / 255.);
		result.b = double(((convertedHex >>  0) & 0xFF) / 255.);
		result.a = 1.;
	}
	else if (sanitizedValue.length() == 8)
	{
		result.r = double(((convertedHex >> 24) & 0xFF) / 255.);
		result.g = double(((convertedHex >> 16) & 0xFF) / 255.);
		result.b = double(((convertedHex >>  8) & 0xFF) / 255.);
		result.a = double(((convertedHex >>  0) & 0xFF) / 255.);
	}
	else
		TUR_CORE_WARN("Invalid HEX value used for color.");
	
	return result;
}

namespace tur
{
	Color::Color(const glm::vec4& color)
		: m_Color(color.r, color.g, color.b, color.a)
	{
		TUR_ASSERT(color.x <= 1.f, "Color float values should be in the range of 0 to 1.");
		TUR_ASSERT(color.x >= 0.f, "Color float values should be in the range of 0 to 1.");

		TUR_ASSERT(color.y <= 1.f, "Color float values should be in the range of 0 to 1.");
		TUR_ASSERT(color.y >= 0.f, "Color float values should be in the range of 0 to 1.");

		TUR_ASSERT(color.z <= 1.f, "Color float values should be in the range of 0 to 1.");
		TUR_ASSERT(color.z >= 0.f, "Color float values should be in the range of 0 to 1.");

		TUR_ASSERT(color.w <= 1.f, "Color float values should be in the range of 0 to 1.");
		TUR_ASSERT(color.w >= 0.f, "Color float values should be in the range of 0 to 1.");
	}

	Color::Color(const glm::vec3& color)
		: m_Color(color.r, color.g, color.b, 1.)
	{
		TUR_ASSERT(color.x <= 1.f, "Color float values should be in the range of 0 to 1.");
		TUR_ASSERT(color.x >= 0.f, "Color float values should be in the range of 0 to 1.");

		TUR_ASSERT(color.y <= 1.f, "Color float values should be in the range of 0 to 1.");
		TUR_ASSERT(color.y >= 0.f, "Color float values should be in the range of 0 to 1.");

		TUR_ASSERT(color.z <= 1.f, "Color float values should be in the range of 0 to 1.");
		TUR_ASSERT(color.z >= 0.f, "Color float values should be in the range of 0 to 1.");
	}

	Color::Color(double r, double g, double b, double a)
		: m_Color{ r, g, b, a }
	{
		TUR_ASSERT(r <= 1.f, "Color float values should be in the range of 0 to 1.");
		TUR_ASSERT(r >= 0.f, "Color float values should be in the range of 0 to 1.");

		TUR_ASSERT(g <= 1.f, "Color float values should be in the range of 0 to 1.");
		TUR_ASSERT(g >= 0.f, "Color float values should be in the range of 0 to 1.");

		TUR_ASSERT(b <= 1.f, "Color float values should be in the range of 0 to 1.");
		TUR_ASSERT(b >= 0.f, "Color float values should be in the range of 0 to 1.");

		TUR_ASSERT(a <= 1.f, "Color float values should be in the range of 0 to 1.");
		TUR_ASSERT(a >= 0.f, "Color float values should be in the range of 0 to 1.");
	}

	Color::Color(U32 r, U32 g, U32 b, U32 a)
		: m_Color { r / 255., g / 255. , b / 255. , a / 255. }
	{
		TUR_ASSERT(r <= 255, "Color integer values should be in the range of 0 to 255.");
		TUR_ASSERT(r >= 0, "Color integer values should be in the range of 0 to 255.");

		TUR_ASSERT(g <= 255, "Color integer values should be in the range of 0 to 255.");
		TUR_ASSERT(g >= 0, "Color integer values should be in the range of 0 to 255.");

		TUR_ASSERT(b <= 255, "Color integer values should be in the range of 0 to 255.");
		TUR_ASSERT(b >= 0, "Color integer values should be in the range of 0 to 255.");

		TUR_ASSERT(a <= 255, "Color integer values should be in the range of 0 to 255.");
		TUR_ASSERT(a >= 0, "Color integer values should be in the range of 0 to 255.");
	}

	Color::Color(int r, int g, int b, int a)
		: m_Color({ r / 255., g / 255., b / 255., a / 255. })
	{
		TUR_ASSERT(r <= 255, "Color integer values should be in the range of 0 to 255.");
		TUR_ASSERT(r >= 0, "Color integer values should be in the range of 0 to 255.");

		TUR_ASSERT(g <= 255, "Color integer values should be in the range of 0 to 255.");
		TUR_ASSERT(g >= 0, "Color integer values should be in the range of 0 to 255.");

		TUR_ASSERT(b <= 255, "Color integer values should be in the range of 0 to 255.");
		TUR_ASSERT(b >= 0, "Color integer values should be in the range of 0 to 255.");

		TUR_ASSERT(a <= 255, "Color integer values should be in the range of 0 to 255.");
		TUR_ASSERT(a >= 0, "Color integer values should be in the range of 0 to 255.");
	}

	Color::Color(const char* hex)
	{
		m_Color = ConvertHexToRGBA(hex);
	}

	glm::vec4 Color::RGBA() const
	{
		return { U32(m_Color.r * 255), U32(m_Color.g * 255),  U32(m_Color.b * 255), U32(m_Color.a * 255) };
	}

	const Color Color::Black	   = Color(0, 0, 0, 255);
	const Color Color::White	   = Color(255, 255, 255, 255);
	const Color Color::Red		   = Color(255, 0, 0, 255);
	const Color Color::Green	   = Color(0, 255, 0, 255);
	const Color Color::Blue		   = Color(0, 0, 255, 255);
	const Color Color::Yellow	   = Color(255, 255, 0, 255);
	const Color Color::Magenta	   = Color(255, 0, 255, 255);
	const Color Color::Cyan		   = Color(0, 255, 255, 255);
	const Color Color::Transparent = Color(0, 0, 0, 0);
}