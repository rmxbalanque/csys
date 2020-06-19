//
// Created by antimatter on 5/28/20.
//

#ifndef CCLI_STRING_H
#define CCLI_STRING_H

#pragma once

#include <string>
#include <cctype>
#include "ccli/ccli_api.h"

namespace ccli
{
	struct CCLI_API String
	{
		String() = default;
		String([[maybe_unused]] const char *str) : m_String(str ? str:"") {}
		String(std::string str) : m_String(std::move(str)) {}
		operator const char*() { return m_String.c_str(); }
		operator std::string() { return m_String; }

		// Moves until first non-whitespace char, returns the index at that spot
		std::pair<size_t, size_t> NextPoi(size_t &start) const
		{
			size_t end = m_String.size();
			std::pair<size_t, size_t> range(end + 1, end);
			size_t pos = start;
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

		[[nodiscard]] size_t End() const { return m_String.size() + 1; }

		std::string m_String;
	};
}

#endif //CCLI_CCLI_STRING_H
