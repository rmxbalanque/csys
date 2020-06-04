#ifndef CCLI_HISTORY_H
#define CCLI_HISTORY_H
#pragma once

namespace ccli
{
	class CCLI_API CommandHistory
	{
	public:

		explicit CommandHistory(unsigned int maxRecord = 100) : m_Record(0), m_MaxRecord(maxRecord), m_History(maxRecord)
		{}

		template<typename T>
		void push_back(const T &line)
		{
			m_History[m_Record++ % m_MaxRecord] = line;
		}

		template<typename T>
		void push_back(const T &&line)
		{
			m_History[m_Record++ % m_MaxRecord] = line;
		}

		const std::string &operator[](unsigned int index);

		const std::string &operator[](int index);

		friend std::ostream &operator<<(std::ostream &os, const CommandHistory &history);

		size_t size();

	private:
		unsigned int m_Record;
		unsigned int m_MaxRecord;
		std::vector<std::string> m_History;
	};
}

#ifdef CCLI_HEADER_ONLY
#include "ccli_history.inl"
#endif

#endif //CCLI_HISTORY_H
