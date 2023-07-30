#pragma once
#include "Graphics/API/IGraphicsAPI.h"

namespace tur
{
	// Todo: use a settings file to set the api data.
	class OGL_API : public IGraphicsAPI
	{
	public:
		struct APIData
		{
			int versionMajor = 4;
			int versionMinor = 3;
		};

	public:
		OGL_API(const APIData& apiData = {})
			: m_APIData(apiData)
		{
			
		}

	public:
		void Shutdown() override
		{
			glfwTerminate();
		}

	public:
		APIData GetAPIData() const { return m_APIData; }

	private:
		APIData m_APIData;
	};
}