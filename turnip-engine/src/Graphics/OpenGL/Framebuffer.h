#pragma once
#include "pch.h"
#include "GraphicsCommon.h"

namespace tur
{
	class Framebuffer
	{
	public:
		Framebuffer(const glm::vec2& size);

		~Framebuffer();

	public:
		void Initialize(const glm::vec2& size);

		void Destroy();

	public:
		inline void Bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_ID); }

		inline void Unbind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	private:
		glm::vec2 m_Size;
		U32 m_ID = TUR_GL_INVALID, m_TextureID = TUR_GL_INVALID, m_RenderBufferID = TUR_GL_INVALID;
	};
}