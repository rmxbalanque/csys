#include <vector>
#include <string>
#include "ccli.h"

int main(void)
{
	using namespace ccli;
	System s;
	s.registerCommand("name", "disc", [](std::vector<String> v){
		for (auto &it : v)
			std::cout << '|' << it.m_String << '|' << std::endl;
	}, Arg<std::vector<String>>("arg_name"));

//	s.registerCommand("name", "disc", [](bool v){
//		std::cout << (v ? "true":"false") << std::endl;
//	}, Arg<bool>("arg_name"));

//	s.registerCommand("name1", "disc", [](std::vector<std::vector<int>> v){
//		std::cout << v.size();
//	}, Arg<std::vector<std::vector<int>>>("arg_name"));

//	String str1 = "[ [1 2 3] [4 5 6] [7 8 9] ]";
//	String str2 = "[[0] [1]]";
//	String str21 = "[ [0] [1] ]";
//	String str22 = "[[0 2 3] [1 2 3]]";
//	String str222 = "[ [ [1] ] [ [2] ] ]";
//	String str2 = "true";
//	String str3 = "[[1 2 3] [4 5 6] [7 8 9]]";
//	std::cout << std::endl << s.commands()["name"]->operator()(str1).m_Data;
//	std::cout << std::endl << s.commands()["name"]->operator()(str2).m_Data;
//	std::cout << std::endl << s.commands()["name"]->operator()(str21).m_Data;
//	std::cout << std::endl << s.commands()["name"]->operator()(str22).m_Data;
//	std::cout << std::endl << s.commands()["name"]->operator()(str222).m_Data;
//	std::cout << std::endl << s.commands()["name"]->operator()(str3).m_Data;

		String str3 = "[\"23456789\" \"98765432\"]";
	  std::cout << std::endl << s.commands()["name"]->operator()(str3).m_Data;

//	std::cout << s.commands()["name"]->Help();
//	std::cout << s.commands()["name1"]->Help();
	return 0;
}