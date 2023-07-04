#pragma once
#include "pch.h"

namespace tur
{
	class Color
	{
	public:
		Color(double r, double g, double b, double a = 1.f);

		Color(U32 r, U32 g, U32 b, U32 a = 255);

		Color(int r, int g, int b, int a = 255);

		Color(const char* hex);

		Color(U64 hex);

	public:
		glm::vec4 RGBA() const;

	public:
		inline float r() const { return m_Color.r; }

		inline float g() const { return m_Color.g; }

		inline float b() const { return m_Color.b; }

		inline float a() const { return m_Color.a; }

	private:
		glm::vec4 m_Color = {};
	};
}