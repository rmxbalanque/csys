

#pragma once

#ifndef CCLI_HEADER_ONLY
#include "ccli_system.h"
#endif

namespace ccli
{
	///////////////////////////////////////////////////////////////////////////
	// Public methods /////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////

	CCLI_INLINE void System::parse(const std::string &line)
	{
		if (line.empty())
			return;

		// Push to history.
		m_CommandHistory.push_back(line);

		// Find command string.
		size_t pos = line.find_first_not_of(' ');
		size_t endpos = line.find_first_of(' ', pos);
		auto str = line.substr(pos, endpos);

		// Set or get
		if (str == "set" || str == "get")
		{
			// Get specialized command and argument.
			endpos = line.find_first_of(' ', endpos + 1);
			String spec_command_name = line.substr(pos, endpos).data();
			String arg = line.substr(endpos).data();

			// Set/Get Command
			auto command = m_CommandContainer.find(spec_command_name.m_String);
			if (command != m_CommandContainer.end())
				(*command->second)(arg);
			else
				std::cout << "ERROR" << std::endl;
		}
		else
		{
			// Normal Command
			auto command = m_CommandContainer.find(str.data());
			String arg = endpos == str.npos ? "" : line.substr(endpos).data();
			if (command != m_CommandContainer.end())
				(*command->second)(arg);
			else
				std::cout << "ERROR" << std::endl;
		}
	}
}