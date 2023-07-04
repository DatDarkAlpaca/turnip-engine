#pragma once

namespace tur
{
	class TurnipApplication
	{
	public:
		TurnipApplication() = default;
		
		~TurnipApplication() = default;

		virtual ~TurnipApplication() = default;

	public:
		virtual void Initialize() { }

		virtual void Update() { }

		virtual void Shutdown() { }
	};
}

#define CREATE_APPLICATION(applicationClass, ...)		\
tur::TurnipApplication* CreateApp()						\
{														\
	return new applicationClass(__VA_ARGS__);			\
}