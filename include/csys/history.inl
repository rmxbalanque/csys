
#pragma once

#ifndef CSYS_HEADER_ONLY

#include "csys/history.h"

#endif

#include <algorithm>
#include <iostream>

namespace csys
{
	///////////////////////////////////////////////////////////////////////////
	// Public methods /////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CSYS_INLINE CommandHistory::CommandHistory(unsigned int maxRecord) : m_Record(0), m_MaxRecord(maxRecord), m_History(maxRecord)
	{
	}

	CSYS_INLINE void CommandHistory::push_back(const std::string &line)
	{
		m_History[m_Record++ % m_MaxRecord] = line;
	}

	CSYS_INLINE unsigned int CommandHistory::get_new_index() const
	{
		return (m_Record - 1) % m_MaxRecord;
	}

	CSYS_INLINE const std::string &CommandHistory::get_new()
	{
		return m_History[(m_Record - 1) % m_MaxRecord];
	}

	CSYS_INLINE unsigned int CommandHistory::get_old_index() const
	{
		if (m_Record <= m_MaxRecord)
			return 0;
		else
			return m_Record % m_MaxRecord;
	}

	CSYS_INLINE const std::string &CommandHistory::get_old()
	{
		if (m_Record <= m_MaxRecord)
			return m_History.front();
		else
			return m_History[m_Record % m_MaxRecord];
	}

	CSYS_INLINE void CommandHistory::clear()
	{
		m_Record = 0;
	}

	CSYS_INLINE const std::string &CommandHistory::operator[](size_t index)
	{
		return m_History[index];
	}

	CSYS_INLINE std::ostream &operator<<(std::ostream &os, const CommandHistory &history)
	{
		os << "History: " << '\n';
		for (unsigned int i = 0; i < history.m_Record && history.m_Record <= history.m_MaxRecord; ++i)
			std::cout << history.m_History[i] << '\n';
		return os;
	}

	CSYS_INLINE size_t CommandHistory::size()
	{
		return m_Record < m_MaxRecord ? m_Record : m_MaxRecord;
	}

	CSYS_INLINE size_t CommandHistory::capacity()
	{
		return m_History.capacity();
	}
}