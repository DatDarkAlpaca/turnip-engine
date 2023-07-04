#pragma once
#include "pch.h"
#include "GraphicsCommon.h"

namespace tur
{
	enum class TextureFilter
	{
		NEAREST = GL_NEAREST,
		LINEAR = GL_LINEAR,
		LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
	};

	enum class TextureWrap
	{
		REPEAT = GL_REPEAT,
		MIRROED_REPEAT = GL_MIRRORED_REPEAT,
		CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
		CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
	};

	struct TextureProps
	{
		U32 width, height, numChannels;
	};

	struct TextureOptions
	{
		TextureFilter minFilter = TextureFilter::LINEAR_MIPMAP_LINEAR, maxFilter = TextureFilter::LINEAR_MIPMAP_LINEAR;
		TextureWrap wrap_s = TextureWrap::REPEAT, wrap_t = TextureWrap::REPEAT;
	};

	class Texture
	{
	public:
		Texture(const char* filepath, TextureOptions options = {});

		Texture() = default;

		~Texture();

	public:
		void Initialize(const char* filepath, TextureOptions options = {});

		void Destroy();

	private:
		void Setup();

	public:
		inline void Bind() { glBindTexture(GL_TEXTURE_2D, m_ID); }

		inline void Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

	public:
		inline TextureProps GetProperties() const { return m_Properties; }

	private:
		TextureProps m_Properties;
		TextureOptions m_Options;

		unsigned char* m_Data;
		U32 m_ID = TUR_GL_INVALID;
	};
}