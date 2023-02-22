#include "cgpch.h"
#include "CodeGenerator.h"

int main()
{
	std::string file_path;
	std::getline(std::cin, file_path);
	std::fstream inp;
	inp.open(file_path, std::ios::in);

	if (inp.fail())
	{
		std::cout << "Fail to open file \"" << file_path << "\".";
		inp.close();
		return -1;
	}

	std::string in, out;
	while (std::getline(inp, in) && std::getline(inp, out))
	{
		CodeGenerator(in, out);
	}

	inp.close();
	return 0;
}