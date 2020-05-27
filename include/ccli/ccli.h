#ifndef CCLI_H
#define CCLI_H

#pragma once

#include "iostream"
#include "ccli_api.h"
#include "ccli_command.h"

void func(int){}

struct CCLI_API Shared
{
	void Test()
	{
	  using namespace ccli;
	  registerCommand("Some_Name", "Some_Description", func, Arg<int>("Age"));
		std::cout << "Successful shared library." << std::endl;
	}
};

#endif
