#pragma once

#include <string>
#include <vector>
#include <deque>
#include <fstream>

class CodeGenerator
{
public:
	static CodeGenerator& Get() { static CodeGenerator s_Instance; return s_Instance; }

	void Generate();
	void Generate(const std::string& input, const std::string& output);
private:
	void GenerateCode();

	void PollInstruction();

	void GetInstructions();

	void IncludeLib(const std::string& engine);
	void IncludeScript(const std::string& script);

	void BeginBlock();
	void EndBlock();

	void AddScripts();
	void OnFunction(std::string& func);
	void OnComponent(std::string& component);
	void OnCreateObject(std::string& ins);

	void EndLine();
	void Tab(uint8_t num);
	void Assign(const std::string& var, const std::string& val);

	std::string GetInstruction();
	std::string GetWord(std::string& instruction);
	std::string GetWord(std::string& instruction, char break_point);
	std::string GetLink(std::string& instruction);
	std::string GetNameFromWord(const std::string& word);

private:
	bool isOnFunction = false;

	std::vector<std::string> scripts;

	std::deque<std::string> instructions;

	std::fstream read;
	std::fstream write;
};