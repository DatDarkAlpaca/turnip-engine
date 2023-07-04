#pragma once
#include "pch.h"
#include "GraphicsCommon.h"

namespace tur
{
	class Buffer
	{
	public:
		Buffer(U32 target);

		~Buffer();

	public:
		void Initialize(size_t sizeInBytes, const void* data, GLenum usage = GL_STATIC_DRAW);

		void Destroy();

	public:
		void SetData(U64 offset, U32 sizeInBytes, const void* data);

	public:
		inline void Bind() const { glBindBuffer(m_Target, m_ID); }

		inline void Unbind() const { glBindBuffer(m_Target, 0); }

	public:
		bool IsValid() const { return m_ID != TUR_GL_INVALID; }

	private:
		U32 m_ID = TUR_GL_INVALID;
		U32 m_Target = TUR_GL_INVALID;
	};
}