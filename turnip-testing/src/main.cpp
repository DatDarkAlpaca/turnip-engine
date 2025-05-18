#include "pch.hpp"

using namespace tur;

class TestView : public tur::View
{
public:
	void on_engine_startup() override
	{
		{
			show_window(&r_Engine->get_window());
			quadRenderer = &r_Engine->get_quad_renderer_system();
		}

		quadRenderer->get().set_clear_color({ 0.20f, 0.20f, 0.20f, 1.f });
		quadRenderer->set_camera(&camera);
		quadRenderer->set_scene(&scene);
	}

	void on_render() override
	{
		quadRenderer->render();
	}

private:
	QuadRendererSystem* quadRenderer;
	tur::Camera camera;
};

int main()
{
	TurnipEngine engine;
	engine.initialize("res/engine_config.json");
	engine.add_view(tur::make_unique<TestView>());
	engine.run();
}
