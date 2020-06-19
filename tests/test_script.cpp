#include "doctest.h"
#include "ccli/ccli_script.h"
#include <fstream>
#include <filesystem>

TEST_CASE("Test CCLI System Class")
{
	/*using namespace ccli;

	std::filesystem::path temp_script_path = std::filesystem::current_path();

	temp_script_path /= "temp_script.sh";
	std::ofstream temp_script_file(temp_script_path.c_str());

	if (temp_script_file.is_open())
	{
		temp_script_file << "help\n";
		temp_script_file << "run for help\n";
		temp_script_file << "set time 10\n";
		temp_script_file << "get time\n";
		temp_script_file.close();
	}

	Script script_(temp_script_path.c_str());
	for (const auto & cmd : script_.data())
	{
		std::cout << cmd << std::endl;
	}*/
}