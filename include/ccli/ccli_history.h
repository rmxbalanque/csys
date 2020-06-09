#ifndef CCLI_HISTORY_H
#define CCLI_HISTORY_H
#pragma once

namespace ccli
{
	// TODO: Avoid looping back since that breaks the history recency.
	class CCLI_API CommandHistory
	{
	public:

		/*!
		 * \brief Command history constructor.
		 * \param maxRecord Maximum amount of command strings to keep track at once.
		 */
		explicit CommandHistory(unsigned int maxRecord = 100) : m_Record(0), m_MaxRecord(maxRecord), m_History(maxRecord)
		{}

		/*!
		 * \brief Record command string. (Start at the beggining once end is reached).
		 * \param line Command string to be recorded.
		 */
		void push_back(const std::string & line);

		/*!
		 * \brief Clear command history
		 */
		void clear();

		/*!
		 * \brief Retrieve command history at given index
		 * \param index Position to lookup in command history vector
		 * \return Command at given index
		 */
		const std::string &operator[](unsigned int index);
		const std::string &operator[](int index);

		/*!
		 * \brief Output available command history.
		 * \param os Output stream
		 * \param history Reference to history to be printed
		 * \return Reference to history to be printed
		 */
		friend std::ostream &operator<<(std::ostream &os, const CommandHistory &history);

		/*!
		 * \return Size of available command history.
		 */
		size_t size();

	private:
		unsigned int m_Record;				//!< Amount of commands recorded
		unsigned int m_MaxRecord;			//!< Maximum command record to keep track of
		std::vector<std::string> m_History; //!< Console command history
	};
}

#ifdef CCLI_HEADER_ONLY
#include "ccli_history.inl"
#endif

#endif //CCLI_HISTORY_H
