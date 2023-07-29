#pragma once
#include "Common.h"

#ifdef TUR_PLATFORM_WINDOWS

#define WIN32_LEAN_AND_MEAN
#define NODEFERWINDOWPOS
#define NOGDICAPMASKS
#define NOSYSCOMMANDS
#define NOSYSMETRICS
#define NOTEXTMETRIC
#define NORASTEROPS
#define OEMRESOURCE
#define NOPROXYSTUB
#define NOCLIPBOARD
#define NODRAWTEXT
#define NOPROFILER
#define NOMETAFILE
#define NOOPENFILE
#define NOSERVICE
#define NOCTLMGR
#define NOMEMMGR
#define NOSCROLL
#define NOMINMAX
#define NOKERNEL
#define NOSOUND
#define NOCOLOR
#define NOIMAGE
#define NOKANJI
#define NOMENUS
#define NOICONS
#define NOCOMM
#define NOATOM
#define NOHELP
#define STRICT
#define NOTAPE
#define NORPC
#define NONLS
#define NOMCX
#define NOWH

#include <Windows.h>

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

#endif