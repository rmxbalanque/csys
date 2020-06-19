//
// Created by antimatter on 5/26/20.
//

#ifndef CCLI_ARGUMENTS_H
#define CCLI_ARGUMENTS_H
#pragma once

#include "ccli/ccli_pch.h"
#include "ccli/ccli_api.h"
#include "ccli/ccli_string.h"
#include "ccli/ccli_exceptions.h"
#include "ccli/ccli_argument_parser.h"
#include <vector>

namespace ccli
{
	template<typename T>
	struct CCLI_API ArgData
	{
		explicit ArgData(String name) : m_Name(std::move(name)), m_Value() {}
		const String m_Name = "";
		String m_TypeName = "Unsupported Type";
		T m_Value;
	};

  template<typename T>
  struct CCLI_API Arg
  {
    using ValueType = T;
    explicit Arg(const String &name) : m_Arg(name) {}
    Arg<T>& Parse(String &input, size_t &start)
    {
    	if (start == input.End())
    		throw Exception("Not enough arguments were given", input.m_String);
    	m_Arg.m_Value = ArgumentParser<ValueType>(input, start).m_Value;
      return *this;
    }

    std::string Info()
		{
    	return std::string(" [") + m_Arg.m_Name.m_String + ":" + m_Arg.m_TypeName.m_String + "]";
		}

		ArgData<ValueType> m_Arg;
  };

#define ARG_BASE_SPEC(TYPE, TYPE_NAME) \
  template<>\
	struct CCLI_API ArgData<TYPE> \
	{ \
		explicit ArgData(String name) : m_Name(std::move(name)), m_Value() {} \
		const String m_Name; \
		String m_TypeName = TYPE_NAME; \
		TYPE m_Value; \
	};

	ARG_BASE_SPEC(String,             "String")
	ARG_BASE_SPEC(bool,               "Boolean")
	ARG_BASE_SPEC(char,               "Char")
	ARG_BASE_SPEC(unsigned char,      "Unsigned_Char")
	ARG_BASE_SPEC(short,              "Signed_Short")
	ARG_BASE_SPEC(unsigned short,     "Unsigned_Short")
	ARG_BASE_SPEC(int,                "Signed_Int")
	ARG_BASE_SPEC(unsigned int,       "Unsigned_Int")
	ARG_BASE_SPEC(long,               "Signed_Long")
	ARG_BASE_SPEC(unsigned long,      "Unsigned_Long")
	ARG_BASE_SPEC(long long,          "Signed_Long_Long")
	ARG_BASE_SPEC(unsigned long long, "Unsigned_Long_Long")
	ARG_BASE_SPEC(float,              "Float")
	ARG_BASE_SPEC(double,             "Double")
	ARG_BASE_SPEC(long double,        "Long_Double")

	template<typename T>
	struct CCLI_API ArgData<std::vector<T>>
	{
		explicit ArgData(String name) : m_Name(std::move(name)) {}
		const String m_Name;
		String m_TypeName = std::string("Vector_Of_") + ArgData<T>("").m_TypeName.m_String;
		std::vector<T> m_Value;
	};

  // TODO: Give better feedback and errors when they mess up
}

#endif //CCLI_ARGUMENTS_H
