#pragma once
#include "Platform/Platform.h"

#include "Graphics/API/GraphicsAPI_Type.h"
#include "Graphics/API/IGraphicsAPI.h"

#ifdef TUR_PLATFORM_WINDOWS
	#include "Platform/Windows/WIN32_Loader.h"
	#include "Platform/OpenGL/OGL_API.h"

#else
	#include "Platform/GLFW/GLFW_Loader.h"
	#include "Platform/OpenGL/OGL_API.h"

#endif

namespace tur
{
	inline std::unique_ptr<IGraphicsAPI> SelectGraphicsAPI(GraphicsAPI_Type api)
	{
		switch (api)
		{
			case GraphicsAPI_Type::NONE:
			{
				TUR_CORE_ERROR("No graphics API has been specified.");
				return nullptr;
			} break;

			case GraphicsAPI_Type::OPENGL: return std::make_unique<OGL_API>();

			case GraphicsAPI_Type::D3D11:
			{
				TUR_CORE_CRITICAL("D3D11 is not implemented yet");
				return nullptr;
			} break;

			case GraphicsAPI_Type::D3D12:
			{
				TUR_CORE_CRITICAL("D3D12 is not implemented yet");
				return nullptr;
			} break;

			case GraphicsAPI_Type::VULKAN:
			{
				TUR_CORE_CRITICAL("Vulkan is not implemented yet");
				return nullptr;
			} break;

			default:
			{
				TUR_CORE_CRITICAL("Failed to select a valid graphics API");
				return nullptr;
			} break;
		}
	}

	inline std::unique_ptr<IGraphicsAPI_Loader> SelectGraphicsAPI_Loader(GraphicsAPI_Type api)
	{
		switch (api)
		{
			case GraphicsAPI_Type::NONE:
			{
				TUR_CORE_CRITICAL("No graphics API loader has been specified.");
				return nullptr;
			} break;

			case GraphicsAPI_Type::OPENGL: return std::make_unique<OGL_GraphicsLoader>();

			case GraphicsAPI_Type::D3D11: return std::make_unique<D3D11_GraphicsLoader>();

			case GraphicsAPI_Type::D3D12:
			{
				TUR_CORE_CRITICAL("D3D12 is not implemented yet");
				return nullptr;
			} break;

			case GraphicsAPI_Type::VULKAN:
			{
				TUR_CORE_CRITICAL("Vulkan is not implemented yet");
				return nullptr;
			} break;

			default:
			{
				TUR_CORE_CRITICAL("Failed to select a valid graphics API");
				return nullptr;
			} break;
		}
	}
}