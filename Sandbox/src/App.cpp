#include <Hudi.h>
#include <Hudi/Core/EntryPoint.h>
#include <glm/glm.hpp>

#include <imgui.h>

#include "EditorLayer.h"

class Sandbox : public Hudi::Application
{
public:
	Sandbox()
	{
		HD_WARN("Currently disable timer!");
		Hudi::Time::SetFPS(30);

		PushLayer(new Hudi::EditorLayer());

		Hudi::SceneManager::Get().CreateNewScene("Default");
		auto& scene = Hudi::SceneManager::Get().GetActiveScene();
		scene.CreateGameObject("vu dep trai");
		scene.CreateGameObject("trai");
		
		auto p_vu = scene.GetGameObjectByRef("vu dep trai");
		auto p_v = scene.GetGameObjectByRef("trai");
		p_vu->AddChild(p_v);
		p_v->SetParent(p_vu);
	}
	~Sandbox()
	{
	}

	Hudi::WindowProps WindowProperties() override;

private:

};


Hudi::WindowProps Sandbox::WindowProperties()
{
	Hudi::WindowProps props;
	props.title = "Sandbox";
	props.width = 800;
	props.height = 600;
	props.flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI;
	return props;
}

Hudi::Application* Hudi::CreateApplication()
{
	return new Sandbox();
}
	