//
// Created by antimatter on 6/10/20.
//

#ifndef CCLI_ARGUMENT_PARSER_H
#define CCLI_ARGUMENT_PARSER_H
#pragma once

#include "ccli_api.h"
#include "ccli_string.h"
#include "ccli_exceptions.h"
#include <vector>
#include <stdexcept>

namespace ccli
{
	// unary argument
	template<typename T>
	struct CCLI_API ArgumentParser
	{
		inline ArgumentParser(String &input, unsigned long &start);
		T m_Value;
	};

	template<typename T>
	inline ArgumentParser<T>::ArgumentParser(String &input, unsigned long &start)
	{
		// getting rid of warnings
		input;start;

//		static_assert(, "Unsupported type T"); // ADD THIS SO PEOPLE DONT USE THEIR OWN TYPES
		throw ArgumentException("Unsupported type: " + std::string(typeid(T).name()));
	}

#define ARG_PARSE_BASE_SPEC(TYPE) \
  template<> \
  struct CCLI_API ArgumentParser<TYPE> \
  { \
    inline ArgumentParser(String &input, unsigned long &start); \
    TYPE m_Value = 0; \
  }; \
  inline ArgumentParser<TYPE>::ArgumentParser(String &input, unsigned long &start)

#define ARG_PARSE_GENERAL_SPEC(TYPE, TYPE_NAME, FUNCTION) \
  ARG_PARSE_BASE_SPEC(TYPE) \
  { \
    auto range = input.NextPoi(start); \
    std::string arg = input.m_String.substr(range.first, range.second); \
    try \
    { \
      m_Value = (TYPE)FUNCTION(arg); \
    } \
    catch (const std::out_of_range&) { throw ArgumentException(std::string("Argument too large for ") + TYPE_NAME, arg); } \
    catch (const std::invalid_argument&) { throw ArgumentException(std::string("Missing or invalid ")\
                                                                    + TYPE_NAME + " argument", arg); } \
  }

	ARG_PARSE_BASE_SPEC(ccli::String)
	{
		auto range = input.NextPoi(start);
		if (input.m_String[range.first] != '"')
			throw ArgumentException("Invalid string argument", input.m_String.substr(range.first, range.second));
		++range.first;
		while (true)
		{
			range.second = input.m_String.find('"', range.first);
			if (range.second == std::string::npos)
				throw ArgumentException("Invalid string argument", m_Value);

			m_Value.m_String += input.m_String.substr(range.first, range.second - 2);
			std::cout << "value = |" << m_Value.m_String << '|' << std::endl;

			if (range.second != (unsigned long)input.m_String.size() - 1)
			{
				if (isspace(input.m_String[range.second + 1]))
					break;
				else
					while(input.m_String[range.second + 1] == '"' && range.second < input.End() - 1)
						++range.second;
				range.first = range.second;
			}
			else if (range.second == (unsigned long)input.m_String.size() - 1)
				break;
		}
		start = range.second + 1;
	}

	ARG_PARSE_BASE_SPEC(bool)
	{
		static const char *s_err_msg = "Missing or invalid boolean argument";
		static const char *s_true = "true\0";
		static const char *s_false = "false";

		// Get argument
		auto range = input.NextPoi(start);
		std::string arg = input.m_String.substr(range.first, range.second);

		// check if the length is between the len of "true" and "false"
		if (arg.length() > 5 || arg.length() < 4)
			throw ArgumentException(s_err_msg, arg);

		// make all letters lower case
		for (size_t i = 0, len = arg.length(); i < len; ++i)
		{
			arg[i] = char(std::tolower(arg[i]));

			// if the current letter is not a letter of "true" or "false", error out
			if (arg[i] != s_true[i] && arg[i] != s_false[i])
				throw ArgumentException(s_err_msg, input.m_String.substr(range.first, range.second));
		}

		if (arg == "true") m_Value = true;
		else if (arg == "false") m_Value = false;
		else ArgumentException(s_err_msg, input.m_String.substr(range.first, range.second));
	}

	ARG_PARSE_BASE_SPEC(char)
	{
		auto range = input.NextPoi(start);
		std::string arg = input.m_String.substr(range.first, range.second);
		// must follow '<char>' convention
		if (arg.length() == 3 && arg[0] == '\'' && arg[2] == '\'')
			m_Value = arg[1];
		else
			throw ArgumentException("Missing or invalid char argument", arg);
	}

	ARG_PARSE_BASE_SPEC(unsigned char)
	{
		auto range = input.NextPoi(start);
		std::string arg = input.m_String.substr(range.first, range.second);
		// must follow '<char>' convention
		if (arg.length() == 3 && arg[0] == '\'' && arg[2] == '\'')
			m_Value = (unsigned char) arg[1];
		else
			throw ArgumentException("Missing or invalid char argument", arg);
	}

	// add the type name to the error message
	ARG_PARSE_GENERAL_SPEC(short, "signed short", std::stoi)

	ARG_PARSE_GENERAL_SPEC(unsigned short, "unsigned short", std::stoul)

	ARG_PARSE_GENERAL_SPEC(int, "signed int", std::stoi)

	ARG_PARSE_GENERAL_SPEC(unsigned int, "unsigned int", std::stoul)

	ARG_PARSE_GENERAL_SPEC(long, "long", std::stol)

	ARG_PARSE_GENERAL_SPEC(unsigned long, "unsigned long", std::stoul)

	ARG_PARSE_GENERAL_SPEC(long long, "long long", std::stoll)

	ARG_PARSE_GENERAL_SPEC(unsigned long long, "unsigned long long", std::stoull)

	ARG_PARSE_GENERAL_SPEC(float, "float", std::stof)

	ARG_PARSE_GENERAL_SPEC(double, "double", std::stod)

	ARG_PARSE_GENERAL_SPEC(long double, "long double", std::stold)


	// [[1 2 3] [4 5 6] [7 8 9]]
	// VV     ^  V     ^ V     ^^
	// V = call, ^ = return

	// input : [[1 2 3] [4 5 6] [7 8 9]]
	// call1 : [1 2 3] [4 5 6] [7 8 9]]
	// call2 : 1 2 3 ] [4 5 6] [7 8 9]
	// call3 : [1 2 3]
	// call4 : 1 2 3

	// IDEAL
	// input : [[1 2 3] [4 5 6]] [[1 2 3] [4 5 6]]
	// call1 : [[1 2 3] [4 5 6] [7 8 9]]
	// call2 : [1 2 3] [4 5 6] [7 8 9]
	// call3 : [1 2 3]
	// call4 : 1 2 3

	// vector arguments, syntax = [arg1 arg2 arg3 ... argN]
	template<typename T>
	struct CCLI_API ArgumentParser<std::vector<T>>
	{
		ArgumentParser(String &input, unsigned long &start);

		std::vector<T> m_Value;
	};

	// if arg], strip ] and pass in arg, return
	// if [arg, strip [ and pass in arg, call

	template<typename T>
	ArgumentParser<std::vector<T>>::ArgumentParser(String &input, unsigned long &start)
	{
		// case 1, [something]
		// case 2, [something something1]

		auto range = input.NextPoi(start);
		if (range.first == input.End() - 1) return;
		if (input.m_String[range.first] != '[')
			throw ArgumentException("Invalid vector argument missing [ delimiter before",
															input.m_String.substr(range.first, range.second));

		input.m_String[range.first] = ' ';

		range = input.NextPoi(range.first);
		start = range.first;

		std::cout << input.m_String << " : [start] = " << input.m_String[start] << std::endl;
		while (range.first < input.End() - 1)
		{
			start = range.first;
			std::cout << input.m_String << " : [start] = " << input.m_String[start] << std::endl;

			if (input.m_String[start] == '[')
			{
				m_Value.push_back(ArgumentParser<T>(input, start).m_Value);
			}
			else if (input.m_String[range.second - 1] == ']')
			{
				if (range.first == range.second - 1)
				{
					++start;
					return;
				}
				while (input.m_String[range.second - 1] == ']' && range.first != range.second)
					--range.second;
				input.m_String[range.second] = ' ';

			  std::cout << input.m_String << " : [start] = " << input.m_String[start] << std::endl;
			  if (!std::isspace(input.m_String[start]))
					m_Value.push_back(ArgumentParser<T>(input, start).m_Value);
				return;
			}
			else
			{
				m_Value.push_back(ArgumentParser<T>(input, start).m_Value);
			}
			range = input.NextPoi(start);
		}
	}
}

#endif //CCLI_ARGUMENT_PARSER_H
