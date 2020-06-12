//
// Created by antimatter on 6/9/20.
//

#include "doctest.h"
#include "ccli_system.h"
#include <cstring>

TEST_CASE("String Argument")
{
	using namespace ccli;
	System s;

#pragma region CORRECT USAGE SINGLE WORD
	s.registerCommand("0", "", [](String str) {
			CHECK(str.m_String == "Zero");
		}, Arg<String>(""));

	s.registerCommand("1", "", [](const char *str) {
			CHECK(!strcmp(str, "One"));
		}, Arg<String>(""));

	s.registerCommand("2", "", [](std::string str) {
			CHECK(str == "Two");
		}, Arg<String>(""));

	// single word strings
	s.runCommand("0 \"Zero\"");
	s.runCommand("0 \"Ze\"ro\"");
	s.runCommand("0 \"Ze\"\"ro\"");

	s.runCommand("1 \"One\"");
	s.runCommand("2 \"Two\"");
#pragma endregion

#pragma region CORRECT USAGE MANY WORDS
	s.registerCommand("0,1", "", [](String str, String str1) {
						CHECK((str.m_String == "Zero" && str1.m_String == "One"));
	}, Arg<String>(""), Arg<String>(""));

	// multi word strings
	s.runCommand("0,1 \"Zero\" \"One\"");
#pragma endregion

#pragma region CORRECT USAGE VECTOR OF MULTI WORD(S)
	s.registerCommand("0,1,2", "", [](std::vector<String> strs) {
		std::string ar[] = { "Zero", "One", "Two"};
		for (unsigned i = 0; i < 3; ++i)
			if (strs[i].m_String != ar[i])
			{
				CHECK(false);
				return;
			}
		CHECK(true);
	}, Arg<std::vector<String>>(""));

	// multi word strings
	s.runCommand("0,1,2 [\"Zero\" \"One\" \"Two\"]");
#pragma endregion

#pragma region CORRECT USAGE VECTOR OF VECTOR OF WORD(S)

#pragma endregion

#pragma region CORRECT USAGE VECTOR OF VECTOR OF VECTOR OF WORD(S)

#pragma endregion
}