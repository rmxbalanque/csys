//
// Created by antimatter on 5/26/20.
//

#ifndef CCLI_ARGUMENTS_H
#include "ccli_pch.h"
#include "base.h"
#include "ccli.h"
#include "ccli_string.h"

namespace ccli
{
	template<typename T>
	struct ArgData
	{
		const string &m_Name;
		string m_TypeName;
		T m_Value;
	};

	template<typename T>
	T ParseArg(string &input, unsigned long &start)
	{
		return T();
	}

  template<typename T>
  struct CCLI_API Arg
  {
    using ValueType = T;
    explicit Arg(const string &name) : m_Arg{name} {}
    Arg<T>& Parse(string &input, unsigned long &start)
    {
    	if (start == input.m_End) throw "Not enough args";
    	m_Arg.m_Value = ParseArg<ValueType>(input, start);
      return *this;
    }

    std::string Info()
		{
    	return "";
		}

		ArgData<ValueType> m_Arg;
  };

#define ARGDATA_SPEC(type, type_name) \
	template<> \
	struct ArgData<type> \
	{ \
		const string &m_Name; \
		string m_TypeName = type_name; \
		type m_Value; \
	}; \
	template<> \
	type ParseArg<type>(string &input, unsigned long &start)

	ARGDATA_SPEC(bool, "Boolean")
	{
		auto range = input.POI(start);
		std::string boolean(input.m_String.begin() + range.first,
												input.m_String.begin() + range.second /*+ (range.second == input.m_End ? -1:0)*/);
		for (auto &c : boolean) c = char(std::tolower(c));
		start = range.second;

		if (boolean == "true") return true;
		if (boolean == "false") return false;
		throw "Missing or invalid boolean argument";
	}

	ARGDATA_SPEC(char, "Char")
	{
		auto range = input.POI(start);
		if (range.first == input.m_End || range.second != range.first + 1)
			throw "Missing or invalid char argument";
		start = range.second;
		return input[range.first];
	}

	ARGDATA_SPEC(unsigned char, "Char")
	{
		auto range = input.POI(start);
		if (range.first == input.m_End || range.second != range.first + 1)
			throw "Missing or invalid char argument";
		start = range.second;
		return input[range.first];
	}

#define ARGDATA_SPEC_GENERAL(function, type, large_message, invalid_message) \
	{ \
		auto range = input.POI(start); \
		try \
		{ \
			type result = (type)function(std::string(input.m_String.begin() + range.first, input.m_String.begin() + range.second)); \
			start = range.second; \
			return result; \
		} \
		catch (const std::out_of_range&) { throw large_message; } \
		catch (const std::invalid_argument&) { throw invalid_message; } \
	}

	ARGDATA_SPEC(short, "Short")
	ARGDATA_SPEC_GENERAL(std::stoi, short,
					"Too large for signed int", "Invalid or missing argument")

	ARGDATA_SPEC(unsigned short, "Unsigned Short")
	ARGDATA_SPEC_GENERAL(std::stoul, unsigned short,
					"Too large for unsigned int", "Invalid or missing argument")

	ARGDATA_SPEC(int, "Int")
	ARGDATA_SPEC_GENERAL(std::stoi, int,
					"Too large for signed int", "Invalid or missing argument")

	ARGDATA_SPEC(unsigned int, "Unsigned Int")
	ARGDATA_SPEC_GENERAL(std::stoul, unsigned int,
											 "Too large for unsigned int", "Invalid or missing argument")

	ARGDATA_SPEC(long, "Long")
	ARGDATA_SPEC_GENERAL(std::stol, long,
											 "Too large for signed long", "Invalid or missing argument")

	ARGDATA_SPEC(unsigned long, "Unsigned Long")
	ARGDATA_SPEC_GENERAL(std::stoul, unsigned long,
											 "Too large for unsigned long", "Invalid or missing argument")

	ARGDATA_SPEC(long long, "Long Long")
	ARGDATA_SPEC_GENERAL(std::stoll, long long,
											 "Too large for signed long long", "Invalid or missing argument")

	ARGDATA_SPEC(unsigned long long, "Unsigned Long Long")
	ARGDATA_SPEC_GENERAL(std::stoull, unsigned long long,
											 "Too large for unsigned long long", "Invalid or missing argument")

	ARGDATA_SPEC(float, "Float")
	ARGDATA_SPEC_GENERAL(std::stof, float,
											 "Too large for float", "Invalid or missing argument")

	ARGDATA_SPEC(double, "Float")
	ARGDATA_SPEC_GENERAL(std::stod, double,
											 "Too large for float", "Invalid or missing argument")

	ARGDATA_SPEC(long double, "Float")
	ARGDATA_SPEC_GENERAL(std::stold, long double,
											 "Too large for float", "Invalid or missing argument")

	ARGDATA_SPEC(ccli::string, "String") { return ""; }

	// arrays

  // TODO: Client input -> command system (strip name) -> command(client input) -> arguments(client input) (strip own arg)
  // TODO: Use own char* not string
  // TODO: Parse commandline for 'strings' and "strings"
  // TODO: Support arrays with {} and [] style
  // TODO: to override, if T = {string (accout for '' or ""), arrays with [] and {}, bool (for true and false (optional caps))}
  // TODO: Give better feedback and errors when they mess up
  // TODO: If given MORE arguments than expected, it drops the rest (should give an error and message)
}
#define CCLI_ARGUMENTS_H

#endif //CCLI_ARGUMENTS_H
