//
// Created by antimatter on 5/28/20.
//

#ifndef CCLI_STRING_H
#define CCLI_STRING_H

#pragma once

#include <string>
#include <cctype>
#include "ccli_api.h"

namespace ccli
{
	struct CCLI_API String
	{
		String() = default;
		String(const char *str) : m_String(str) {}
		String(std::string str) : m_String(std::move(str)) {}
		operator const char*() { return m_String.c_str(); }
		operator std::string() { return m_String; }

		// Moves until first non-whitespace char, returns the index at that spot
		std::pair<unsigned long, unsigned long> NextPoi(unsigned long &start)
		{
			unsigned long end = (unsigned long)m_String.size() + 1;
			std::pair<unsigned long, unsigned long> range(end, end - 1);
			unsigned long pos = start;
			for (; pos < end; ++pos)
				if (!std::isspace(m_String[pos]))
				{
					range.first = pos;
					break;
				}
			for (; pos < end; ++pos)
				if (std::isspace(m_String[pos]))
				{
					range.second = pos;
					break;
				}

			start = range.second;
			return range;
		}

		[[nodiscard]] unsigned long End() const { return (unsigned long)m_String.size() + 1; }

		std::string m_String;
	};
}

#endif //CCLI_CCLI_STRING_H
