#ifndef CCLI_H
#define CCLI_H

#pragma once
#include "iostream"
#include "ccli_api.h"
#include "ccli_command.h"
#include <string>

struct CCLI_API Shared
{
	void Test()
	{
	  using namespace ccli;
		auto command = registerCommand("Run", "Starts the game",
						[](bool b, bool c, char d, char e, short x, unsigned short y){
			std::cout << (b && c) << " " << d << " " << e << " " << x << " " << y << std::endl;
			},
			Arg<bool>("B"), Arg<bool>("C"),
			        Arg<unsigned char>("D"), Arg<char>("E"),
			                Arg<short>("X"), Arg<unsigned short>("Y"));

		string input1 = "true true a p 123 321";
		string input2 = "true false b l 123 123";
		string input3 = "false true c k 23 4";
		string input4 = "false false d j 62122 32";
		(*command)(input1);
		(*command)(input2);
		(*command)(input3);
		(*command)(input4);
	}
};

#endif
