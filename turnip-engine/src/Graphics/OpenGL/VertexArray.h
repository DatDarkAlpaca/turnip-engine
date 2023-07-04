#pragma once
#include "pch.h"
#include "Attribute.h"
#include "GraphicsCommon.h"

namespace tur
{
	class VertexArray
	{
	public:
		VertexArray();

		~VertexArray();

	public:
		void Destroy();

	public:
		void AddAttribute(const Attribute& attribute);

		void FlushLayout();

	public:
		inline void Bind() const { glBindVertexArray(m_ID); }

		inline void Unbind() const { glBindVertexArray(0); }

	public:
		inline bool IsValid() const { return m_ID != TUR_GL_INVALID; }

	private:
		std::vector<Attribute> m_Attributes;
		U32 m_ID = TUR_GL_INVALID;
	};
}