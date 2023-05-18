#include "CodeGenerator.h"

#include <iostream>

int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		std::cout << "No input file available!";
		return -1;
	}

	std::fstream inp;
	inp.open(argv[1], std::ios::in);

	if (inp.fail())
	{
		std::cout << "Fail to open file \"" << argv[0] << "\".";
		return -1;
	}

	std::string in, out;
	while (std::getline(inp, in) && std::getline(inp, out))
	{
		CodeGenerator::Get().Generate(in, out);
	}

	inp.close();
	return 0;
}