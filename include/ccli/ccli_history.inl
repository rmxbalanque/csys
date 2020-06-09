
#pragma once

#ifndef CCLI_HEADER_ONLY
#include "ccli_history.h"
#endif

namespace ccli
{
	///////////////////////////////////////////////////////////////////////////
	// Public methods /////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CCLI_INLINE void CommandHistory::push_back(const std::string &line)
	{
		m_History[m_Record++ % m_MaxRecord] = line;
	}

	CCLI_INLINE void CommandHistory::clear()
	{
		m_Record = 0;
		m_History.clear();
	}

	CCLI_INLINE const std::string &CommandHistory::operator[](unsigned int index)
	{
		return m_History[index];
	}

	CCLI_INLINE const std::string &CommandHistory::operator[](int index)
	{
		return m_History[static_cast<unsigned int>(index)];
	}

	CCLI_INLINE std::ostream &operator<<(std::ostream &os, const CommandHistory &history)
	{
		os << "History: " << '\n';
		for (unsigned int i = 0; i < history.m_Record && history.m_Record <= history.m_MaxRecord; ++i)
			std::cout << history.m_History[i] << '\n';
		return os;
	}

	CCLI_INLINE size_t CommandHistory::size()
	{ return m_Record < m_MaxRecord ? m_Record : m_MaxRecord; }
}