//
// Created by antimatter on 5/28/20.
//

#ifndef CCLI_STRING_H
#include <string>
namespace ccli
{
	struct string
	{
		string() = default;
		string(const char *str) : m_String(str), m_End(m_String.size() + 1) {}
		operator const char*() { return m_String.c_str(); }

		// Moves until first non-whitespace char, returns the index at that spot, -1 if end
		std::pair<unsigned long, unsigned long> POI(unsigned long start = 0)
		{
			std::pair<unsigned long, unsigned long> range(m_End, m_End - 1);
			unsigned long pos = start;
			for (; pos < m_End; ++pos)
				if (!std::isspace(m_String[pos]))
				{
					range.first = pos;
					break;
				}
			for (; pos < m_End; ++pos)
				if (std::isspace(m_String[pos]))
				{
					range.second = pos;
					break;
				}

			return range;
		}
		std::string m_String;
		unsigned long m_End;
	};
}

#define CCLI_STRING_H

#endif //CCLI_CCLI_STRING_H
