

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
		// Error.
		if (line.empty())
			return;

		// Push to history.
		m_CommandHistory.push_back(line);

		// Find command string.
		size_t cmd_pos = line.find_first_not_of(' ');
		if (cmd_pos == std::string::npos) return;
		size_t cmd_endpos = line.find_first_of(' ', cmd_pos);
		auto str = line.substr(cmd_pos, cmd_endpos);

		// Set or get
		if (str == "set" || str == "get")	// TODO: Store "set"/"get" as const string references.
		{
			// Get specialized command and argument.

			// No variable provided.
			if (line.length() == cmd_endpos || cmd_endpos == std::string::npos)
			{
				// TODO: Log error.
				std::cout << "ERROR: No variable provided" << '\n';
				return;
			}

			// Get command.
			size_t var_endpos = line.find_first_of(' ', cmd_endpos + 1);
			String spec_command_name = line.substr(cmd_pos, var_endpos).data();

			// No argument provided.
			if (var_endpos == std::string::npos)
			{
				// TODO: Log error.
				std::cout << "ERROR: No argument provided" << '\n';
				return;
			}

			// Get arg.
			size_t arg_pos = line.find_first_not_of(' ', var_endpos);
			if (arg_pos == line.length() || (arg_pos - var_endpos != 1)|| arg_pos == std::string::npos)
			{
				std::cout << "ERROR: Not a valid argument" << '\n';
				return;
			}
			size_t arg_endpos = line.find_first_of(' ', arg_pos + 1);

			// No argument provided.
			if (arg_endpos == std::string::npos && (arg_pos - arg_endpos) == 0)
			{
				// TODO: Log error.
				std::cout << "ERROR: No argument provided" << '\n';
				return;
			}

			// Get argument.
			String arg = line.substr(arg_pos, arg_endpos).data();

			// TODO: Check if this should be made optional.
			if (arg_endpos != std::string::npos)
			{
				// TODO: Log error.
				std::cout << "ERROR: More arguments than required were specified." << '\n';
			}

			// Command not found.
			auto command = m_CommandContainer.find(spec_command_name.m_String);
			if (command == m_CommandContainer.end())
			{
				std::cout << "ERROR: Command doesn't exist." << std::endl;
				return;
			}
			// Execute command.
			(*command->second)(arg);
		}
		else
		{
			// Normal Command
			auto command = m_CommandContainer.find(str.data());

			// Command not found.
			if (command == m_CommandContainer.end())
			{
				std::cout << "ERROR: Command doesn't exist." << std::endl;
				return;
			}

			// Rest of the line.
			String arg = cmd_endpos == str.npos ? "" : line.substr(cmd_endpos).data();

			// Execute command.
			(*command->second)(arg);
		}
	}
}