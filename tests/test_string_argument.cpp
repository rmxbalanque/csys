//
// Created by antimatter on 6/9/20
//

#include "doctest.h"
#include "ccli/ccli_system.h"
#include <cstring>

TEST_CASE("String Argument")
{
	using namespace ccli;
	System s;

// CORRECT USAGE SINGLE WORD
	String strt;
	s.registerCommand("0", "", [&strt](String str) {
		  strt = str;
//			std::cout << "String -> " << str.m_String << std::endl;
		}, Arg<String>(""));

	s.registerCommand("1", "", [](const char *str) {
			CHECK(!strcmp(str, "One"));
		}, Arg<String>(""));

	s.registerCommand("2", "", [](std::string str) {
			CHECK(str == "Two");
		}, Arg<String>(""));

	s.registerCommand("3", "", [](std::string str) {
						CHECK(str == "");
	}, Arg<String>(""));

	// single word strings
	s.runCommand("0 Zero\\]"); // Zero\] -> Zero\]
	CHECK((strt.m_String == "Zero]"));
	strt.m_String.clear();

	s.runCommand("0 \"Zero\\\"\""); // Zero\] -> Zero\]
  CHECK((strt.m_String == "Zero\""));
	strt.m_String.clear();

	s.runCommand("0 \"Zero \\\" \\\\\""); // Zero\] -> Zero\]
					CHECK((strt.m_String == "Zero \" \\"));
	strt.m_String.clear();

	s.runCommand("0 \"Zero\"\"One\"\"    #    \""); // Zero\] -> Zero\]
					CHECK((strt.m_String == "ZeroOne    #    "));
	strt.m_String.clear();

// CORRECT USAGE MANY WORDS
	s.registerCommand("0,1", "", [](String str, String str1) {
		bool zero = str.m_String == "Zero";
		bool one = str1.m_String == "One";
		CHECK(zero);
		CHECK(one);
	}, Arg<String>(""), Arg<String>(""));

	// multi word strings
	s.runCommand("0,1 \"Zero\" \"One\"");
	s.runCommand("0,1     Zero    One    ");

// CORRECT USAGE VECTOR OF MULTI WORD(S)
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
	s.runCommand("0,1,2 [  \"Zero\" \"One\" \"Two\"   ]");

// CORRECT USAGE VECTOR OF VECTOR OF MULTI WORD(S)
	s.registerCommand("0,1,2,3", "", [](std::vector<std::vector<String>> strs) {
		std::vector<std::vector<std::string>> arr = { {"One", "Two"}, {" |Three| |Yeet|"}, { " Four]", "FIVE?" } };
		CHECK((strs[0][0].m_String == arr[0][0]));
		CHECK((strs[0][1].m_String == arr[0][1]));
		CHECK((strs[1][0].m_String == arr[1][0]));
		CHECK((strs[2][0].m_String == arr[2][0]));
		CHECK((strs[2][1].m_String == arr[2][1]));
	}, Arg<std::vector<std::vector<String>>>(""));

	// multi word strings
	s.runCommand("0,1,2,3 [[One Two] [\" |Three| |Yeet|\"] [\" Four\\]\" FIVE?]]");

	s.registerCommand("char0", "", [](char c) { CHECK(c == '"'); }, Arg<char>(""));
	s.registerCommand("char1", "", [](char c) { CHECK(c == '"'); }, Arg<char>(""));
	s.registerCommand("char2", "", [](char c) { CHECK(c == '\\'); }, Arg<char>(""));
	s.registerCommand("char3", "", [](char c) { CHECK(c == 'a'); }, Arg<char>(""));
	s.registerCommand("char4", "", [](char c) { CHECK(c == 'b'); }, Arg<char>(""));

	// multi word strings
	s.runCommand("char0 \"");   // \ issue
	s.runCommand("char0 ");   // \ issue
	s.runCommand("char0 a\"");  // too many
	s.runCommand("char0 aa");   // too many
	s.runCommand("char0 aaa");   // too many
	s.runCommand("char1 \\\""); // " good
	s.runCommand("char2 \\\\"); // \ good
	s.runCommand("char3 a");    // a good
	s.runCommand("char4 b");    // b good
}