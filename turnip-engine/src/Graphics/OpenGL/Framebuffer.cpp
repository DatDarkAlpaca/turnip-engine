#include "pch.h"
#include "Framebuffer.h"

namespace tur
{
	Framebuffer::Framebuffer(const glm::vec2& size)
	{
		Initialize(size);
	}

	Framebuffer::~Framebuffer()
	{
		if (m_ID == TUR_GL_INVALID && m_TextureID == TUR_GL_INVALID && m_RenderBufferID == TUR_GL_INVALID)
			return;

		TUR_CORE_WARN("Framebuffer instance destructor called, but object was never destroyed. It is advised to use Destroy() beforehand.");

	}

	void Framebuffer::Initialize(const glm::vec2& size)
	{
		m_Size = size;

		glGenFramebuffers(1, &m_ID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TextureID, 0);

		glGenRenderbuffers(1, &m_RenderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x, size.y);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);

		U32 code = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (code != GL_FRAMEBUFFER_COMPLETE)
		{
			TUR_CORE_ERROR("Failed to create framebuffer. Error code: {}", code);
			return;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::Destroy()
	{
		TUR_ASSERT(m_ID != TUR_GL_INVALID && m_TextureID != TUR_GL_INVALID && m_RenderBufferID != TUR_GL_INVALID,
			"Attempted to destroy a non-initialized framebuffer."
		);

		glDeleteFramebuffers(1, &m_ID);
		m_ID = TUR_GL_INVALID;
		m_TextureID = TUR_GL_INVALID;
		m_RenderBufferID = TUR_GL_INVALID;
	}
}