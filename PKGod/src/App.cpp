#include <Hudi.h>
#include <Hudi/Core/EntryPoint.h>

class PKGod : public Hudi::Application
{
public:
	PKGod() {}
	~PKGod() {}

	void Preprocess() override {}
};

Hudi::Application* Hudi::CreateApplication()
{
	return new PKGod();
}