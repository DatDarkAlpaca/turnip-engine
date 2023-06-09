#include "pch.h"
#include "VertexArray.h"

#pragma warning(push)
#pragma warning(disable: 4312)

namespace tur
{
	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &m_ID);
		glBindVertexArray(m_ID);
	}

	VertexArray::~VertexArray()
	{
		if (m_ID == TUR_GL_INVALID)
			return;

		Destroy();
	}

	void VertexArray::Destroy()
	{
		TUR_ASSERT(m_ID != TUR_GL_INVALID, "Attempted to destroy a non-initialized VAO.");

		glDeleteVertexArrays(1, &m_ID);
		m_ID = TUR_GL_INVALID;
	}

	void VertexArray::AddAttribute(const Attribute& attribute)
	{
		m_Attributes.push_back(attribute);
	}

	void VertexArray::FlushLayout()
	{
		auto sum = [&](int lhs, const Attribute& rhs) -> U32
		{
			return (U32)(lhs + rhs.size * GetTypeSize(rhs.type));
		};

		U32 stride = std::accumulate(m_Attributes.begin(), m_Attributes.end(), (U32)0, sum);
		U32 offset = 0;

		for (const auto& attribute : m_Attributes)
		{
			glEnableVertexAttribArray(attribute.location);

			glVertexAttribPointer(
				attribute.location,
				attribute.size,
				attribute.type,
				attribute.normalized,
				stride,
				(const void*)offset
			);

			offset += attribute.size * (U32)GetTypeSize(attribute.type);
		}

		m_Attributes.clear();
	}
}

#pragma warning(pop)