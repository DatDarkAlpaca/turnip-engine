#include "pch.h"
#include "Texture.h"
#include "Logger/Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace
{
	static std::vector<glm::vec3> GenerateDefaultTexture(tur::TextureProps props)
	{
		const glm::vec3 magenta = { 1.f, 0.f, 1.f };
		const glm::vec3 white   = { 1.f, 1.f, 1.f };

		std::vector<glm::vec3> data;
		data.resize(props.width * props.height);

		for (U32 y = 0; y < props.height; ++y)
		{
			if (y % 2 == 0)
				for (U32 x = 0; x < props.width; ++x)
				{
					if (x % 2 == 0)
						data[x + y * props.width] = magenta;
					else
						data[x + y * props.width] = white;
				}

			else
				for (U32 x = 0; x < props.width; ++x)
				{
					if (x % 2 == 0)
						data[x + y * props.width] = white;
					else
						data[x + y * props.width] = magenta;
				}
		}

		return data;
	}
}

namespace tur
{
	Texture::Texture(const char* filepath, TextureOptions options)
	{
		Initialize(filepath, options);
	}

	Texture::~Texture()
	{
		
	}

	void Texture::Initialize(const char* filepath, TextureOptions options)
	{
		m_Options = options;

		int width, height, numChannels;
		stbi_set_flip_vertically_on_load(true);
		m_Data = stbi_load(filepath, &width, &height, &numChannels, 0);

		if (!m_Data)
			TUR_CORE_WARN("Failed to initialize texture from path: {}.", filepath);

		if (width <= 0 || height <= 0 || numChannels < 0)
		{
			width = 4;
			height = 4;
		}

		m_Properties = TextureProps{ (U32)width, (U32)height, (U32)numChannels };

		Setup();

		stbi_image_free(m_Data);
	}

	void Texture::Setup()
	{
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     (U32)m_Options.wrap_s);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     (U32)m_Options.wrap_t);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (U32)m_Options.minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (U32)m_Options.maxFilter);

		int dataFormat = 0;
		{
			if (m_Properties.numChannels == 4)
				dataFormat = GL_RGBA;
			else if (m_Properties.numChannels == 3)
				dataFormat = GL_RGB;
		}

		if (m_Data && dataFormat != 0)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, m_Properties.width, m_Properties.height, 0, dataFormat, GL_UNSIGNED_BYTE, m_Data);
			glGenerateMipmap(GL_TEXTURE_2D);
			
			glBindTexture(GL_TEXTURE_2D, 0);

			return;
		}

		// Failed to load data. Default fallback.
		std::vector<glm::vec3> data = GenerateDefaultTexture(m_Properties);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Properties.width, m_Properties.height, 0, GL_RGB, GL_FLOAT, &data[0]);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Destroy()
	{
		m_ID = TUR_GL_INVALID;
	}	
}