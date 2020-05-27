#ifndef CCLI_H
#define CCLI_H

#pragma once

#include "iostream"
#include "ccli_api.h"
#include "ccli_command.h"
#include <string>

void func(int a, std::string b)
{
  std::cout << "| " << a << " | " << b << " |";
}

struct CCLI_API Shared
{
	void Test()
	{
	  using namespace ccli;
	  registerCommand("Some_Name", "Some_Description", func, Arg<int>(""), Arg<std::string>(""));
    (*CommandBase::s_Commands[0])("1000 test");
    delete CommandBase::s_Commands[0];
//		std::cout << "Successful shared library." << std::endl;
	}
};

#endif
