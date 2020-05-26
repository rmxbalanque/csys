#ifndef CCLI_H
#define CCLI_H

#pragma once

#include "iostream"
#include "ccli_api.h"

struct CCLI_API Shared
{
	void Test()
	{
		std::cout << "Successful shared library." << std::endl;
	}
};

#endif
