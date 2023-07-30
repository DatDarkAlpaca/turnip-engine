#pragma once
#include "Graphics/API/IGraphicsAPI.h"

namespace tur
{
	// Todo: use a settings file to set the api data.
	// Todo: improve the generalization of the api and avoid duplication if possible [WET might be the best option].

#ifdef TUR_WINDOWING_GLFW
	class OGL_API_GLFW : public IGraphicsAPI
	{
	public:
		struct APIData
		{
			int versionMajor = 4;
			int versionMinor = 3;
		};

	public:
		OGL_API_GLFW(const APIData& apiData = {})
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
#endif

#ifdef TUR_PLATFORM_WINDOWS
	class OGL_API_WIN32 : public IGraphicsAPI
	{
		friend class WIN32_Loader_OGL;

	public:
		struct APIData
		{
			int versionMajor = 4;
			int versionMinor = 3;
		};

	public:
		OGL_API_WIN32(const APIData& apiData = {})
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
#endif
}