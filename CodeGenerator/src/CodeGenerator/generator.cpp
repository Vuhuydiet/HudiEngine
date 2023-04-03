#include "cgpch.h"
#include "CodeGenerator.h"

int main(int argc, char** argv)
{
	std::fstream inp;
	inp.open(argv[0], std::ios::in);

	if (inp.fail())
	{
		std::cout << "Fail to open file \"" << argv[0] << "\".";
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