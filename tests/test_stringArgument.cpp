//
// Created by antimatter on 6/9/20.
//

#include "doctest.h"
#include "ccli.h"

TEST_CASE("String Argument")
{
	using namespace ccli;
	System s;
	s.registerCommand("string", "disc", [](std::string s) {
		std::cout << s << std::endl;
	}, Arg<String>("str1"));

//	s.parse("string \"str1\"");
	s.parse("string \"str1\"\"str2\"");
//	s.parse("string ");
}