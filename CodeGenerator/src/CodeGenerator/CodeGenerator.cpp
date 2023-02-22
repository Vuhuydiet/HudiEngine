#include "cgpch.h"
#include "CodeGenerator.h"

CodeGenerator::CodeGenerator()
{
	std::string in_filePath, out_filePath;

	std::cout << "Input file: ";
	std::getline(std::cin, in_filePath);

	read.open(in_filePath, std::ios::in);
	if (read.fail())
		std::cout << "Cannot read.\n";

	std::cout << "Output file: ";
	std::getline(std::cin, out_filePath);

	write.open(out_filePath, std::ios::out);
	if (write.fail())
		std::cout << "Cannot write.\n";

	GenerateCode();
}

CodeGenerator::~CodeGenerator()
{
	write.close();
}

void CodeGenerator::GenerateCode()
{
	GetInstructions();
	PollInstruction();
}

void CodeGenerator::PollInstruction()
{
	while (!instructions.empty())
	{
		std::string ins = GetInstruction();
		auto type = GetWord(ins);
		if (type == "//")
		{
			// do nothing
		}
		else if (type == "#")
		{
			auto name = GetWord(ins);
			write << "#include <" << name << ">"; EndLine();
		}
		else if (type == "##")
		{
			auto name = GetWord(ins);
			scripts.push_back(name);
			write << "#include \"" << name << "\""; EndLine();
		}
		else if (type == "%")
		{
			if (isOnFunction)
				continue;
			auto func = GetWord(ins);
			OnFunction(func);
		}
		else if (type == "*")
		{
			auto name = GetNameFromWord(GetWord(ins));
			Tab(1); Assign("auto& go", "scene.CreateGameObject(\"" + name + "\")");
			AddScripts();
		}
		else if (type == "-")
		{
			OnComponent(ins);
		}
		else if (type == "++")
		{
			OnCreateObject(ins);
		}
	}
}

void CodeGenerator::GetInstructions()
{
	std::string instruction;
	while (std::getline(read, instruction))
	{
		for (auto& c : instruction)
		{
			if (c != ' ' && c != '	')
			{
				instructions.push_back(instruction);
				break;
			}
		}
	}
	read.close();
}

void CodeGenerator::IncludeLib(const std::string& lib)
{
	write << "#include <" << lib << ">"; EndLine();
}

void CodeGenerator::IncludeScript(const std::string& script)
{
	write << "#include \"" << script << "\""; EndLine();
}

void CodeGenerator::BeginBlock()
{
	write << "{"; EndLine();
}

void CodeGenerator::EndBlock()
{
	write << "}"; EndLine();
}

void CodeGenerator::AddScripts()
{
	for (auto& script : scripts)
	{
		while (true)
		{
			if (script[script.size() - 1] == '.')
			{
				script.pop_back();
				break;
			}
			script.pop_back();
		}
		Tab(1);  write << "go.AddComponent<" << script << ">();"; EndLine();
	}
}

void CodeGenerator::OnFunction(std::string& func)
{
	write << "void " << func << "(Scene& scene)"; EndLine();

	isOnFunction = true;
	BeginBlock();
	PollInstruction();
	EndBlock();
	isOnFunction = false;
}

void CodeGenerator::OnComponent(std::string& instruction)
{
	auto component = GetWord(instruction);
	if (component == "Transform")
	{
		Tab(1); Assign("auto& transform", "go.GetComponent<Transform>()");

		while (!instructions.empty())
		{
			auto ins = GetInstruction();
			auto type = GetWord(ins);
			if (type == "+")
			{
				auto attribute = GetWord(ins);
				if (attribute == "position")
				{
					GetWord(ins);
					std::string x = GetWord(ins), y = GetWord(ins), z = GetWord(ins);
					x.pop_back();
					y.pop_back();

					Tab(2); Assign("transform.position.x", x);
					Tab(2); Assign("transform.position.y", y);
					Tab(2); Assign("transform.position.z", z);
				}
				else if (attribute == "rotation")
				{
					GetWord(ins);
					std::string x = GetWord(ins), y = GetWord(ins), z = GetWord(ins);
					x.pop_back();
					y.pop_back();

					Tab(2); Assign("transform.rotation.x", x);
					Tab(2); Assign("transform.rotation.y", y);
					Tab(2); Assign("transform.rotation.z", z);
				}
				else if (attribute == "scale")
				{
					GetWord(ins);
					std::string x = GetWord(ins); x.pop_back();
					std::string y = GetWord(ins); y.pop_back();
					std::string z = GetWord(ins);

					Tab(2); Assign("transform.scale.x", x);
					Tab(2); Assign("transform.scale.y", y);
					Tab(2); Assign("transform.scale.z", z);
				}
			}
			else
			{
				instructions.push_front(type + ' ' + ins);
				return;
			}
		}
	}
	else if (component == "Sprite_Renderer")
	{
		Tab(1); Assign("auto& sprite", "go.GetComponent<SpriteRenderer>();");
		while (!instructions.empty())
		{
			auto ins = GetInstruction();
			auto type = GetWord(ins);
			if (type == "+")
			{
				auto attribute = GetWord(ins);
				if (attribute == "order")
				{
					GetWord(ins);
					auto x = GetWord(ins);
					Tab(2); Assign("sprite.order", x);
				}
				else if (attribute == "filePath")
				{
					GetWord(ins);
					auto path = GetLink(ins);
					Tab(2);  write << "sprite.SetFilePath(" << path << ");"; EndLine();
				}
			}
			else
			{
				instructions.push_front(type + ' ' + ins);
				return;
			}
		}
	}
	else if (component == "Rigid_Body_2D")
	{
		Tab(1); write << "go.AddComponent<RigidBody2D>();"; EndLine();
		Tab(1); Assign("auto& body", "go.GetComponent<RigidBody2D>()");
		while (!instructions.empty())
		{
			auto ins = GetInstruction();
			auto type = GetWord(ins);
			if (type == "+")
			{
				auto attribute = GetWord(ins);
				if (attribute == "bodyType")
				{
					GetWord(ins);
					auto x = GetWord(ins);
					Tab(2);
					if (x == "dynamic")
					{
						Assign("body.bodyType", "RigidBody2D::BodyType::Dynamic");
					}
					else if (x == "static")
					{
						Assign("body.bodyType", "RigidBody2D::BodyType::Static");
					}
				}
				else if (attribute == "gravityScale")
				{
					GetWord(ins);
					auto gc = GetWord(ins);
					Tab(2); Assign("body.gravityScale", gc);
				}
				else if (attribute == "mass")
				{
					GetWord(ins);
					auto mass = GetWord(ins);
					Tab(2); write << "body.SetMass(" << mass << ");"; EndLine();
				}
			}
			else
			{
				instructions.push_front(type + ' ' + ins);
				return;
			}
		}
	}
	else if (component == "Box_Collider_2D")
	{
		Tab(1); write << "go.AddComponent<BoxCollider2D>();"; EndLine();
		Tab(1); Assign("auto& box", "go.GetComponent<BoxCollider2D>()");
		while (!instructions.empty())
		{
			auto ins = GetInstruction();
			auto type = GetWord(ins);
			if (type == "+")
			{
				auto attribute = GetWord(ins);
				if (attribute == "tag")
				{
					GetWord(ins);
					auto tag = GetWord(ins);
					Tab(2); Assign("box.tag", tag);
				}
				else if (attribute == "minPoint")
				{
					GetWord(ins);
					auto x = GetWord(ins); x.pop_back();
					auto y = GetWord(ins);
					Tab(2); Assign("box.minPoint.x", "transform.position.x + " + x);
					Tab(2); Assign("box.minPoint.y", "transform.position.y + " + y);
				}
				else if (attribute == "maxPoint")
				{
					GetWord(ins);
					auto x = GetWord(ins); x.pop_back();
					auto y = GetWord(ins);
					Tab(2); Assign("box.maxPoint.x", "transform.position.x + " + x);
					Tab(2); Assign("box.maxPoint.y", "transform.position.y + " + y);
				}
				else if (attribute == "isDrawn")
				{
					GetWord(ins);
					auto val = GetWord(ins);
					Tab(2); Assign("box.isDrawn", val);
				}
			}
			else
			{
				instructions.push_front(type + ' ' + ins);
				return;
			}
		}
	}
	else if (component == "Animator")
	{
		Tab(1); write << "go.AddComponent<Animator>();"; EndLine();
		Tab(1); Assign("auto& anim", "go.GetComponent<Animator>()");
		while (!instructions.empty())
		{
			auto ins = GetInstruction();
			auto type = GetWord(ins);
			if (type == "+")
			{
				auto attribute = GetWord(ins);
				if (attribute == "add_int")
				{
					GetWord(ins);
					auto name = GetWord(ins);
					Tab(2); write << "anim.Add_Int_Parameter(" << name << ");"; EndLine();
				}
				else if (attribute == "add_float")
				{
					GetWord(ins);
					auto name = GetWord(ins);
					Tab(2); write << "anim.Add_Float_Parameter(" << name << ");"; EndLine();
				}
				else if (attribute == "add_bool")
				{
					GetWord(ins);
					auto name = GetWord(ins);
					Tab(2); write << "anim.Add_Bool_Parameter(" << name << ");"; EndLine();
				}
				else if (attribute == "add_trigger")
				{
					GetWord(ins);
					auto name = GetWord(ins);
					Tab(2); write << "anim.Add_Trigger_Parameter(" << name << ");"; EndLine();
				}
				else if (attribute == "add_animation")
				{
					GetWord(ins);
					auto name = GetNameFromWord(GetWord(ins));
					Tab(2); write << "anim.AddAnimation(" << name << ");"; EndLine();
				}
				else if (attribute == "make_transition")
				{
					GetWord(ins);
					auto name1 = GetWord(ins); name1.pop_back();
					auto name2 = GetWord(ins);
					Tab(2); write << "anim.MakeTransition(" << name1 << ", " << name2 << ");"; EndLine();
				}
				else if (attribute == "add_condition")
				{
					GetWord(ins);
					auto name1 = GetWord(ins); name1.pop_back();
					auto name2 = GetWord(ins); name2.pop_back();
					auto cdn_name = GetWord(ins); cdn_name.pop_back();
					auto val = GetWord(ins);
					Tab(2); write << "anim.GetTransition(" << name1 << ", " << name2
						<< ").AddCondition(" << cdn_name << ", " << val << ");"; EndLine();
				}
			}
			else
			{
				instructions.push_front(type + ' ' + ins);
				return;
			}
		}
	}
}

void CodeGenerator::OnCreateObject(std::string& instruction)
{
	auto object = GetWord(instruction);
	if (object == "Animation")
	{
		auto name = GetNameFromWord(GetWord(instruction));
		Tab(1);  write << object << " " << name << "(\"" << name << "\");"; EndLine();
		while (!instructions.empty())
		{
			auto ins = GetInstruction();
			auto type = GetWord(ins);
			if (type == "+")
			{
				auto attribute = GetWord(ins);
				if (attribute == "filePath")
				{
					GetWord(ins);
					Tab(2); Assign(name + ".filePath", GetWord(ins));
				}
				else if (attribute == "frameTimes")
				{
					GetWord(ins);
					Tab(2);  write << name << ".SetFrameTimes(" << ins << " );"; EndLine();
				}
				else if (attribute == "frameSize")
				{
					GetWord(ins);
					auto x = GetWord(ins); x.pop_back();
					auto y = GetWord(ins);
					Tab(2);  write << name << ".SetFrameSize(" << x << ", " << y << ");"; EndLine();
				}
				else if (attribute == "add_animation_event")
				{
					GetWord(ins);
					auto order = GetWord(ins); order.pop_back();
					auto objectType = GetWord(ins); objectType.pop_back();
					auto method = GetWord(ins); method.pop_back();
					Tab(2);  write << name + ".AddAnimationEvent(" + order + ", std::bind(&" + objectType + "::" + method
						+ ", &" + name + ".GetComponent<" + objectType + ">());"; EndLine();
				}
			}
			else
			{
				instructions.push_front(type + ' ' + ins);
				return;
			}
		}
	}
}

void CodeGenerator::EndLine()
{
	write << '\n';
}

void CodeGenerator::Tab(uint8_t num)
{
	for(int i=0; i<num; i++)
		write << '	';
}

void CodeGenerator::Assign(const std::string& var, const std::string& val)
{
	write << var << " = " << val << ";"; EndLine();
}

std::string CodeGenerator::GetInstruction()
{
	auto s = instructions.front();
	instructions.pop_front();
	return s;
}

std::string CodeGenerator::GetWord(std::string& instruction)
{
	bool flag = false;
	int pos = 0;
	std::string s;
	while (pos < instruction.size())
	{
		if (flag && (instruction[pos] == ' ' || instruction[pos] == '	'))
			break;

		if (instruction[pos] != ' ' && instruction[pos] != '	')
			flag = true;

		if (flag)
		{
			s += instruction[pos];
		}

		pos++;
	}
	instruction = instruction.substr(pos, instruction.size() - pos);

	return s;
}

std::string CodeGenerator::GetWord(std::string& instruction, char break_point)
{	
	bool flag = false;
	int pos = 0;
	std::string s;
	while (pos < instruction.size())
	{
		if (flag && instruction[pos] == break_point)
		{
			pos++;
			break;
		}

		if (instruction[pos] != ' ' && instruction[pos] != '	')
			flag = true;

		if (flag)
		{
			s += instruction[pos];
		}

		pos++;
	}
	instruction = instruction.substr(pos, instruction.size() - pos);

	return s;
}

std::string CodeGenerator::GetLink(std::string& instruction)
{
	std::string s;
	int count = 0;
	int pos = 0;
	for (auto& c : instruction)
	{
		if (c == '"')
			count++;
		if (count)
			s += c;
		pos++;
		if (count == 2)
			break;
	}
	instruction = instruction.substr(pos, instruction.size() - pos);
	return s;
}

std::string CodeGenerator::GetNameFromWord(const std::string& word)
{
	std::string s;
	for (auto& c : word)
	{
		if (c == '\"')
			continue;
		s += c;
	}
	return s;
}

