#include <fstream>
#include <vector>
#include <string>
#include "ccli.h"

int main(void)
{
	using namespace ccli;
	auto command = registerCommand("command", "runs a command that takes an int", [](int, float){}, Arg<int>("Argument1"), Arg<float>("Argument2"));
	auto command1 = registerCommand("empty_command", "runs a command that takes nothing", [](){});

	std::cout << command->Help();
	std::cout << command1->Help();
	return 0;
}