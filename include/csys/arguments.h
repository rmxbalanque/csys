// Copyright (c) 2020-present, Roland Munguia & Tristan Florian Bouchard.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef CSYS_ARGUMENTS_H
#define CSYS_ARGUMENTS_H
#pragma once

#include "csys/api.h"
#include "csys/string.h"
#include "csys/exceptions.h"
#include "csys/argument_parser.h"
#include <vector>

namespace csys
{
	template<typename T>
	struct CSYS_API ArgData
	{
		explicit ArgData(String name) : m_Name(std::move(name)), m_Value() {}
		const String m_Name = "";
		String m_TypeName = "Unsupported Type";
		T m_Value;
	};

  template<typename T>
  struct CSYS_API Arg
  {
    using ValueType = T;
    explicit Arg(const String &name) : m_Arg(name) {}
    Arg<T> &Parse(String &input, size_t &start)
    {
    	size_t index = start;
    	if (input.NextPoi(index).first == input.End())
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

  using NULL_ARGUMENT = void(*)();

  // TODO: ADD TO ENDING ARGUMENT
	template<>
	struct CSYS_API Arg<NULL_ARGUMENT>
	{
		Arg<NULL_ARGUMENT> &Parse(String &input, size_t &start)
		{
			if (input.NextPoi(start).first != input.End())
				throw Exception("Too many arguments were given", input.m_String);
			return *this;
		}
	};

#define ARG_BASE_SPEC(TYPE, TYPE_NAME) \
  template<>\
	struct CSYS_API ArgData<TYPE> \
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
	struct CSYS_API ArgData<std::vector<T>>
	{
		explicit ArgData(String name) : m_Name(std::move(name)) {}
		const String m_Name;
		String m_TypeName = std::string("Vector_Of_") + ArgData<T>("").m_TypeName.m_String;
		std::vector<T> m_Value;
	};

  // TODO: Give better feedback and errors when they mess up
}

#endif //CSYS_ARGUMENTS_H
