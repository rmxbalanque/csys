#include <vector>
#include <string>
#include "ccli.h"

int main(void)
{
	using namespace ccli;
	System s;
	s.registerCommand("name", "disc", [](std::vector<std::vector<int>> v){
		for (auto &it : v)
		{
			for (auto &it2 : it) std::cout << " " << it2;
			std::cout << std::endl;
		}
	}, Arg<std::vector<std::vector<int>>>("arg_name"));

//	s.registerCommand("name1", "disc", [](std::vector<std::vector<int>> v){
//		std::cout << v.size();
//	}, Arg<std::vector<std::vector<int>>>("arg_name"));

//	String str1 = "[ [1 2 3] [4 5 6] [7 8 9] ]";
//	String str2 = "[[0] [1]]";
	String str2 = "[ [1]]";
//	String str3 = "[[1 2 3] [4 5 6] [7 8 9]]";
//	std::cout << std::endl << s.commands()["name"]->operator()(str1).m_Data;
	std::cout << std::endl << s.commands()["name"]->operator()(str2).m_Data;
//	std::cout << std::endl << s.commands()["name"]->operator()(str3).m_Data;

//	std::cout << s.commands()["name"]->Help();
//	std::cout << s.commands()["name1"]->Help();
	return 0;
}