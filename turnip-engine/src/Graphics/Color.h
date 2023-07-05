#pragma once
#include "pch.h"

namespace tur
{
	class Color
	{
	public:
		Color(const glm::vec4& color);

		Color(const glm::vec3& color);

		Color(double r, double g, double b, double a = 1.f);

		Color(U32 r, U32 g, U32 b, U32 a = 255);

		Color(int r, int g, int b, int a = 255);

		Color(const char* hex);

	public:
		glm::vec4 RGBA() const;

	public:
		inline float r() const { return m_Color.r; }

		inline float g() const { return m_Color.g; }

		inline float b() const { return m_Color.b; }

		inline float a() const { return m_Color.a; }

	public:
		static const Color Black;
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Magenta;
		static const Color Cyan;
		static const Color Transparent;

	private:
		glm::vec4 m_Color = {};
	};
}