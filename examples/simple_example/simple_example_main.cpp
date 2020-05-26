#include <iostream>
#include "ccli.h"
#include "ccli_autocomplete.h"

int main(void)
{
	std::cout << "Simple_Test_Program" << std::endl;
	Shared test;
	test.Test();
	ccli::acTernaryTree tree;
	tree.insert("call");
	tree.insert("me");
	tree.insert("mind");
	tree.insert("mid");
	tree.insert("call");
	tree.insert("megan");
	tree.insert("monitoring");
	tree.insert("malfunction");
	tree.insert("malice");
	tree.insert("c");
	tree.insert("corona");
	tree.insert("roland");


	std::vector<std::string> options;
	tree.options("m", options);
	for (const auto & str : options)
	{
		std::cout << str << '\n';
	}
	return 0;
}