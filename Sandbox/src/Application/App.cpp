#include <Hudi.h>
#include <Hudi/Core/EntryPoint.h>

class Sandbox : public Hudi::Application
{
public:
	Sandbox()
	{
	}
	~Sandbox()
	{
	}

	void Preprocess() override;

	Hudi::WindowProps WindowProperties() override;

private:

};

Hudi::Application* Hudi::CreateApplication()
{
	return new Sandbox();
}

Hudi::WindowProps Sandbox::WindowProperties()
{
	Hudi::WindowProps props;
	props.title = "Sandbox";
	props.height = 648;
	props.width = 1152;
	props.flags = 0;
	return props;
}

void MainMenuScene(Hudi::Scene& scene);
void skdfskdhf(Hudi::Scene& scene);
void Sandbox::Preprocess()
{
	Hudi::Time::SetFPS(60);
	
	auto MainMenu = Hudi::SceneManager::CreateNewScene("Main Menu");
	Hudi::SceneManager::GetScene("Main Menu").SetLoadFunction(MainMenuScene);
	Hudi::SceneManager::CreateNewScene("sjdhfksd");

}


