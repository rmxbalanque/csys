#ifndef CCLI_H
#define CCLI_H

#pragma once

#include "iostream"
#include "ccli_api.h"
#include "ccli_command.h"
#include <string>

void CCLI_API func(int a, std::string b)
{
  std::cout << "| " << a << " | " << b << " |";
}

struct CCLI_API Shared
{
	void Test()
	{
	  using namespace ccli;
	  auto command = registerCommand("Some_Name", "Some_Description", func, Arg<int>(""), Arg<std::string>(""));
    (*command)("1000 test");
    delete command;
//		std::cout << "Successful shared library." << std::endl;
	}
};

#endif
