#include "CodeGenerator.h"

#include <iostream>

int main(int argc, char** argv)
{
	std::fstream inp;
	inp.open(argv[1], std::ios::in);

	if (inp.fail())
	{
		std::cout << "Fail to open file \"" << argv[0] << "\".";
		inp.close();
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