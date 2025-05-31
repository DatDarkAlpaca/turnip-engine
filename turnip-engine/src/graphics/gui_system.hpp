#pragma once

namespace tur
{
	template<typename Backend>
	class BaseGUISystem
	{
	public:
		void initialize()
		{
			static_cast<Backend*>(this)->initialize_impl();
		}

	public:
		void begin_frame()
		{
			static_cast<Backend*>(this)->begin_frame_impl();
		}
		void render()
		{
			static_cast<Backend*>(this)->render_impl();
		}
		void end_frame()
		{
			static_cast<Backend*>(this)->end_frame_impl();
		}
	};

	class OpenGLGUI : public BaseGUISystem<OpenGLGUI>
	{
	public:
		void initialize_impl()
		{

		}

	public:
		void begin_frame_impl()
		{
		
		}
		void render_impl()
		{

		}
		void end_frame_impl()
		{

		}
	};
}